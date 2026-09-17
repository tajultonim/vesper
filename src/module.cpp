#include "module.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <utility>

#include "lexer.hpp"
#include "parser.hpp"

ModuleLoader::ModuleLoader(std::filesystem::path rootDirectory)
    : rootDirectory(std::move(rootDirectory)) {}

std::shared_ptr<Module> ModuleLoader::load(const std::string &path) {
  return loadRecursive(path);
}

std::shared_ptr<Module> ModuleLoader::loadRecursive(const std::string &path) {
  // std::cout << "MODULE_LOADER: Loading module: " << path << std::endl;
  // Already completely loaded?
  auto loaded = modules.find(path);

  if (loaded != modules.end()) {
    // std::cout << "MODULE_LOADER: Module already loaded: " << path <<
    // std::endl;
    return loaded->second;
  }

  // Currently loading this module?
  if (loading.find(path) != loading.end()) {
    throw std::runtime_error(
        "MODULE_ERROR: Circular module dependency involving '" + path + "'");
  }

  loading.insert(path);

  std::filesystem::path filePath = rootDirectory / (path + ".vsp");

  std::ifstream file(filePath);

  if (!file) {
    loading.erase(path);

    throw std::runtime_error("MODULE_ERROR: Could not load module '" + path +
                             "'");
  }

  std::stringstream buffer;
  buffer << file.rdbuf();

  std::string source = buffer.str();

  // -------------------------
  // Lex
  // -------------------------

  Lexer lexer(source);
  auto tokens = lexer.tokenize();

  // -------------------------
  // Parse
  // -------------------------

  Parser parser(tokens);
  Program program = parser.parseProgram();

  // -------------------------
  // Recursively load imports
  // -------------------------

  // std::cout << "MODULE_LOADER: Recursively loading imports for module: " <<
  // path
  //           << std::endl;

  for (const auto &statement : program.statements) {
    const auto *import = dynamic_cast<ImportStatement *>(statement.get());

    if (!import)
      continue;

    loadRecursive(import->path);
  }

  // std::cout << "MODULE_LOADER: Finished loading imports for module: " << path
  // << std::endl;

  // -------------------------
  // Finished loading
  // -------------------------

  loading.erase(path);

  auto module = std::make_shared<Module>(path, std::move(program));
  modules.insert_or_assign(path, module);

  return module;
}