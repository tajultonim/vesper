constexpr const char *VESPER_VERSION = "0.1.0";

#include <fstream>
#include <iostream>
#include <sstream>

#include "interpreter.hpp"
#include "lexer.hpp"
#include "module.hpp"
#include "parser.hpp"
#include "resolver.hpp"
#include "token.hpp"
#include "type_checker.hpp"

int main(int argc, char *argv[]) {

  try {
    if (argc < 2) {
      std::cerr << "Usage: vesper [--verbose|-V] [--tokens <file>] <file.vsp>\n"
                << "       vesper --help\n"
                << "       vesper --version\n";
      return 1;
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
          std::cerr << "--tokens requires an output file.\n";
          return 1;
        }

        writeTokensToFile = true;
      } else if (argument.empty()) {
        argument = std::move(option);
      } else {
        std::cerr << "Unexpected argument: " << option << '\n';
        return 1;
      }
    }

    if (argument.empty()) {
      std::cerr << "A source file is required.\n";
      return 1;
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
    std::filesystem::path rootPath = sourcePath.parent_path();

    std::ifstream file(sourcePath);

    if (!file) {
      std::cerr << "Could not open file: " << argument << '\n';
      return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    std::string source = buffer.str();

    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    if (verbose) {
      std::cerr << "[verbose] lexed " << tokens.size() << " tokens\n";
    }

    if (writeTokensToFile && tokenOutputPath.empty()) {
      writeTokens(std::cout, tokens);
    } else if (writeTokensToFile) {
      std::ofstream tokenFile(tokenOutputPath);
      if (!tokenFile) {
        std::cerr << "Could not write token file: " << tokenOutputPath.string()
                  << '\n';
        return 1;
      }

      writeTokens(tokenFile, tokens);

      if (!tokenFile) {
        std::cerr << "Failed to write token file: "
                  << tokenOutputPath.string() << '\n';
        return 1;
      }

      if (verbose) {
        std::cerr << "[verbose] wrote tokens to "
                  << tokenOutputPath.string() << '\n';
      }
    }

    Parser parser(tokens);

    Program program = parser.parseProgram();
    if (verbose) {
      std::cerr << "[verbose] parsed entry program\n";
    }

    ModuleLoader loader(sourcePath.parent_path(), verbose);
    Interpreter interpreter;
    for (const auto &statement : program.statements) {
      if (const auto *import =
              dynamic_cast<const ImportStatement *>(statement.get())) {
        auto module = loader.load(import->path);
        interpreter.registerModule(import->alias, module);
      }
    }

    Resolver resolver;
    resolver.resolveProgram(program);
    if (verbose) {
      std::cerr << "[verbose] resolved names\n";
    }

    TypeChecker checker;
    checker.checkProgram(program);
    if (verbose) {
      std::cerr << "[verbose] type checking passed\n";
    }

    interpreter.execute(program);
    if (verbose) {
      std::cerr << "[verbose] execution finished\n";
    }

    return 0;
  } catch (const std::exception &e) {
    std::cerr << "ERROR: " << e.what() << '\n';
    return 1;
  }
}