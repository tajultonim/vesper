constexpr const char *VESPER_VERSION = "0.1.0";

#include <fstream>
#include <iostream>
#include <sstream>

#include "errors.hpp"
#include "interpreter.hpp"
#include "lexer.hpp"
#include "module.hpp"
#include "parser.hpp"
#include "resolver.hpp"
#include "token.hpp"
#include "type_checker.hpp"

int main(int argc, char *argv[]) {

  std::string sourceFile;

  try {
    if (argc < 2) {
      throw CliError(
          "Usage: vesper [--verbose|-V] [--tokens <file>] <file.vsp>");
    }

    bool verbose = false;
    bool writeTokensToFile = false;
    std::filesystem::path tokenOutputPath;
    std::string argument;

    for (int index = 1; index < argc; ++index) {
      std::string option = argv[index];

      if (option == "--verbose" || option == "-V") {
        verbose = true;
      } else if (option == "--tokens") {
        writeTokensToFile = true;

        if (!argument.empty() && index + 1 < argc &&
            argv[index + 1][0] != '-') {
          tokenOutputPath = argv[++index];
        }
      } else if (option.rfind("--tokens=", 0) == 0) {
        tokenOutputPath = option.substr(9);
        if (tokenOutputPath.empty()) {
          throw CliError("--tokens requires an output file.");
        }

        writeTokensToFile = true;
      } else if (argument.empty()) {
        argument = std::move(option);
      } else {
        throw CliError("Unexpected argument: " + option);
      }
    }

    if (argument.empty()) {
      throw CliError("A source file is required.");
    }

    if (argument == "--help" || argument == "-h") {
      std::cout << "Vesper " << VESPER_VERSION << '\n'
                << '\n'
                << "Usage:\n"
                << "  vesper [--verbose|-V] [--tokens <file>] <file.vsp>\n"
                << "                       Run a Vesper program\n"
                << "  vesper --help        Show this help message\n"
                << "  vesper --version     Show version information\n"
                << "\n"
                << "Options:\n"
                << "  --verbose, -V        Show lexer, parser, and module diagnostics\n"
                << "  --tokens <file>      Write the token dump to a file\n";

      return 0;
    }

    if (argument == "--version" || argument == "-v") {
      std::cout << "Vesper " << VESPER_VERSION << '\n';
      return 0;
    }

    std::filesystem::path sourcePath = argument;
    sourceFile = sourcePath.string();

    std::ifstream file(sourcePath);

    if (!file) {
      throw CliError("Could not open file: " + argument);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    std::string source = buffer.str();

    std::vector<Token> tokens;
    try {
      Lexer lexer(source);
      tokens = lexer.tokenize();
    } catch (const VesperError &) {
      throw;
    } catch (const std::exception &error) {
      rethrowWithSource<LexerError>(error, sourcePath.string());
    }

    if (verbose) {
      std::cerr << "[verbose] lexed " << tokens.size() << " tokens\n";
    }

    if (writeTokensToFile && tokenOutputPath.empty()) {
      writeTokens(std::cout, tokens);
    } else if (writeTokensToFile) {
      std::ofstream tokenFile(tokenOutputPath);
      if (!tokenFile) {
        throw CliError("Could not write token file: " +
                       tokenOutputPath.string());
      }

      writeTokens(tokenFile, tokens);

      if (!tokenFile) {
        throw CliError("Failed to write token file: " +
                       tokenOutputPath.string());
      }

      if (verbose) {
        std::cerr << "[verbose] wrote tokens to "
                  << tokenOutputPath.string() << '\n';
      }
    }

    Program program;
    try {
      Parser parser(tokens);
      program = parser.parseProgram();
    } catch (const VesperError &) {
      throw;
    } catch (const std::exception &error) {
      rethrowWithSource<ParserError>(error, sourcePath.string());
    }
    if (verbose) {
      std::cerr << "[verbose] parsed entry program\n";
    }

    ModuleLoader loader(sourcePath.parent_path(), verbose);
    Interpreter interpreter;
    try {
      for (const auto &statement : program.statements) {
        if (const auto *import =
                dynamic_cast<const ImportStatement *>(statement.get())) {
          auto module = loader.load(import->path);
          interpreter.registerModule(import->alias, module);
        }
      }
    } catch (const VesperError &) {
      throw;
    } catch (const std::exception &error) {
      rethrowWithSource<ModuleError>(error, sourcePath.string());
    }

    Resolver resolver;
    try {
      resolver.resolveProgram(program);
    } catch (const VesperError &) {
      throw;
    } catch (const std::exception &error) {
      rethrowWithSource<ResolverError>(error, sourcePath.string());
    }
    if (verbose) {
      std::cerr << "[verbose] resolved names\n";
    }

    TypeChecker checker;
    try {
      checker.checkProgram(program);
    } catch (const VesperError &) {
      throw;
    } catch (const std::exception &error) {
      rethrowWithSource<TypeError>(error, sourcePath.string());
    }
    if (verbose) {
      std::cerr << "[verbose] type checking passed\n";
    }

    try {
      interpreter.execute(program);
    } catch (const VesperError &) {
      throw;
    } catch (const std::exception &error) {
      rethrowWithSource<RuntimeError>(error, sourcePath.string());
    }
    if (verbose) {
      std::cerr << "[verbose] execution finished\n";
    }

    return 0;
  } catch (const VesperError &error) {
    std::cerr << errorPhaseColor(error.phase()) << "ERROR: "
              << error.formatWithFile(sourceFile) << "\x1b[0m\n";
    return 1;
  } catch (const std::exception &e) {
    std::cerr << "ERROR: " << e.what() << '\n';
    return 1;
  }
}