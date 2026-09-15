#include <stdexcept>
#include <utility>

#include "environment.hpp"

Environment::Environment(std::shared_ptr<Environment> parent)
    : parent(std::move(parent)) {}

void Environment::define(const std::string &name, Variable value) {
  variables[name] = std::move(value);
}

void Environment::assign(const std::string &name, Variable value, int line,
                         int column) {
  auto it = variables.find(name);

  if (it != variables.end()) {
    if (!it->second.mutable_) {
      throw std::runtime_error("Cannot assign to immutable variable '" + name +
                               "' at line " + std::to_string(line) +
                               ", column " + std::to_string(column));
    }

    it->second = std::move(value);
    return;
  }

  // Variable wasn't found locally.
  // Search the parent environment.
  if (parent) {
    parent->assign(name, std::move(value), line, column);
    return;
  }

  throw std::runtime_error("Undefined variable '" + name + "' at line " +
                           std::to_string(line) + ", column " +
                           std::to_string(column));
}

Value Environment::get(const std::string &name, int line, int column) const {
  auto it = variables.find(name);

  if (it != variables.end()) {
    return it->second.value;
  }

  // Variable wasn't found locally.
  // Search the parent environment.
  if (parent) {
    return parent->get(name, line, column);
  }

  throw std::runtime_error("Undefined variable '" + name + "' at line " +
                           std::to_string(line) + ", column " +
                           std::to_string(column));
}