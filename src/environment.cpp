#include <stdexcept>

#include "environment.hpp"

void Environment::define(const std::string &name, int value) {
  variables[name] = value;
}

int Environment::get(const std::string &name, int line, int column) const {
  auto it = variables.find(name);

  if (it == variables.end()) {
    throw std::runtime_error("Undefined variable '" + name + "' at line " +
                             std::to_string(line) + ", column " +
                             std::to_string(column));
  }
  return variables.at(name);
}