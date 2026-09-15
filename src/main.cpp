constexpr const char *VESPER_VERSION = "0.1.0";

#include <fstream>
#include <iostream>
#include <sstream>

#include "interpreter.hpp"
#include "lexer.hpp"
#include "module.hpp"
#include "parser.hpp"
#include "token.hpp"
#include "type_checker.hpp"

int main(int argc, char *argv[]) {

  try {
    if (argc != 2) {
      std::cerr << "Usage: vesper <file.vsp>\n"
                << "       vesper --help\n"
                << "       vesper --version\n";
      return 1;
    }

    std::string argument = argv[1];

    if (argument == "--help" || argument == "-h") {
      std::cout << "Vesper " << VESPER_VERSION << '\n'
                << '\n'
                << "Usage:\n"
                << "  vesper <file.vsp>    Run a Vesper program\n"
                << "  vesper --help        Show this help message\n"
                << "  vesper --version     Show version information\n";

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

    printTokens(tokens);

    Parser parser(tokens);

    Program program = parser.parseProgram();
    ModuleLoader loader(sourcePath.parent_path());
    for (const auto &statement : program.statements) {
      if (const auto *import =
              dynamic_cast<const ImportStatement *>(statement.get())) {
        Module module = loader.load(import->path);

        // For now, just verify it loaded.
        std::cout << "Loaded module: " << import->path << " as "
                  << import->alias << '\n';
      }
    }

    TypeChecker checker;
    checker.checkProgram(program);

    Interpreter interpreter;
    interpreter.execute(program);

    return 0;
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << '\n';
    return 1;
  }
}