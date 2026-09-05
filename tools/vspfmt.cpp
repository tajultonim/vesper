#include "../src/lexer.hpp"
#include "../src/parser.hpp"
#include "./formatter.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

int main(int argc, char *argv[]) {
  if (argc < 2 || argc > 3) {
    std::cerr << "Usage: vspfmt <file.vsp> [-w]\n";
    return 1;
  }

  const std::string filename = argv[1];
  const bool write = argc == 3 && std::string(argv[2]) == "-w";

  if (argc == 3 && !write) {
    std::cerr << "Unknown option: " << argv[2] << '\n';
    return 1;
  }

  std::ifstream file(filename);

  if (!file) {
    std::cerr << "Could not open file: " << filename << '\n';
    return 1;
  }

  std::stringstream buffer;
  buffer << file.rdbuf();

  std::string source = buffer.str();

  try {
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    Program program = parser.parseProgram();

    Formatter formatter;
    std::string formatted = formatter.format(program);

    if (write) {
      std::ofstream output(filename);

      if (!output) {
        std::cerr << "Could not write to file: " << filename << '\n';
        return 1;
      }

      output << formatted;

      if (!output) {
        std::cerr << "Failed to write formatted file: " << filename << '\n';
        return 1;
      }
    } else {
      std::cout << formatted;
    }
  } catch (const std::exception &e) {
    std::cerr << "Formatting failed: " << e.what() << '\n';
    return 1;
  }

  return 0;
}