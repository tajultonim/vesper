#include <stdexcept>

#include "environment.hpp"

void Environment::define(const std::string &name, Variable value) {
  variables[name] = value;
}

void Environment::assign(const std::string &name, Variable value, int line,
                         int column) {
  auto it = variables.find(name);
  if (it == variables.end()) {
    throw std::runtime_error("Undefined variable '" + name + "' at line " +
                             std::to_string(line) + ", column " +
                             std::to_string(column));
  }

  if (!it->second.mutable_) {
    throw std::runtime_error("Cannot assign to immutable variable '" + name +
                             "' at line " + std::to_string(line) + ", column " +
                             std::to_string(column));
  }
  it->second = value;
}

Value Environment::get(const std::string &name, int line, int column) const {
  auto it = variables.find(name);

  if (it == variables.end()) {
    throw std::runtime_error("Undefined variable '" + name + "' at line " +
                             std::to_string(line) + ", column " +
                             std::to_string(column));
  }
  return variables.at(name).value;
}