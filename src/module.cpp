#include "module.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <utility>

#include "lexer.hpp"
#include "parser.hpp"

ModuleLoader::ModuleLoader(std::filesystem::path rootDirectory)
    : rootDirectory(std::move(rootDirectory)) {}

Module ModuleLoader::load(const std::string &path) {
  std::cout << "Loading module: " << path << " from " << rootDirectory
            << std::endl;
  std::filesystem::path filePath = rootDirectory / (path + ".vsp");

  std::ifstream file(filePath);

  if (!file) {
    throw std::runtime_error("Could not load module '" + path + "'");
  }

  std::stringstream buffer;
  buffer << file.rdbuf();

  std::string source = buffer.str();

  Lexer lexer(source);
  auto tokens = lexer.tokenize();

  Parser parser(tokens);
  Program program = parser.parseProgram();

  return Module{path, std::move(program)};
}