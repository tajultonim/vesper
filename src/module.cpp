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
  return loadRecursive(path, rootDirectory);
}

std::shared_ptr<Module> ModuleLoader::loadRecursive(
    const std::string &path,
    const std::filesystem::path &importingDirectory) {
  const std::filesystem::path filePath =
      resolvePath(path, importingDirectory);
  const std::string cacheKey = filePath.generic_string();

  if (loading.find(cacheKey) != loading.end()) {
    throw std::runtime_error(
        "MODULE_ERROR: Circular module dependency involving '" + path + "'");
  }

  auto loaded = modules.find(cacheKey);

  if (loaded != modules.end()) {
    return loaded->second;
  }

  loading.insert(cacheKey);

  std::ifstream file(filePath);

  if (!file) {
    loading.erase(cacheKey);

    throw std::runtime_error("MODULE_ERROR: Could not load module '" +
                 path + "' from " + importingDirectory.string());
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

  auto module = std::make_shared<Module>(path, std::move(program));
  modules.insert_or_assign(cacheKey, module);

  for (const auto &statement : module->program.statements) {
    const auto *import = dynamic_cast<ImportStatement *>(statement.get());

    if (!import)
      continue;

    module->imports[import->alias] =
        loadRecursive(import->path, filePath.parent_path());
  }

  loading.erase(cacheKey);

  return module;
}

std::filesystem::path ModuleLoader::resolvePath(
    const std::string &path,
    const std::filesystem::path &importingDirectory) const {
  std::filesystem::path modulePath(path);

  if (modulePath.extension() != ".vsp") {
    modulePath += ".vsp";
  }

  const std::filesystem::path resolved =
      std::filesystem::weakly_canonical(importingDirectory / modulePath);

  if (!std::filesystem::exists(resolved) ||
      !std::filesystem::is_regular_file(resolved)) {
    throw std::runtime_error("MODULE_ERROR: Could not resolve module '" +
                             path + "' relative to " +
                             importingDirectory.string());
  }

  return resolved;
}