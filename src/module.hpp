#pragma once

#include <filesystem>
#include <string>

#include "ast.hpp"

struct Module {
  std::string path;
  Program program;
};

class ModuleLoader {
public:
  explicit ModuleLoader(std::filesystem::path rootDirectory);

  Module load(const std::string &path);

private:
  std::filesystem::path rootDirectory;
};