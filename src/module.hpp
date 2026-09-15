#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "ast.hpp"

struct Module {
  std::string path;
  Program program;

  Module() = default;
  Module(std::string path, Program program)
      : path(std::move(path)), program(std::move(program)) {}

  Module(const Module &) = delete;
  Module &operator=(const Module &) = delete;
  Module(Module &&) noexcept = default;
  Module &operator=(Module &&) noexcept = default;
};

class ModuleLoader {
public:
  explicit ModuleLoader(std::filesystem::path rootDirectory);

  Module load(const std::string &path);

private:
  Module loadRecursive(const std::string &path);

  std::filesystem::path rootDirectory;

  // Modules that have already been completely loaded.
  std::unordered_map<std::string, Module> modules;

  // Modules currently being loaded.
  std::unordered_set<std::string> loading;
};