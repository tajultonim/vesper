#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "value.hpp"

class Environment {
public:
  explicit Environment(std::shared_ptr<Environment> parent = nullptr);

  void define(const std::string &name, Variable value);

  void assign(const std::string &name, Variable value, int line, int column);

  Value get(const std::string &name, int line, int column) const;

private:
  std::unordered_map<std::string, Variable> variables;

  std::shared_ptr<Environment> parent;
};