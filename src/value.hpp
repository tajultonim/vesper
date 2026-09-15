#pragma once

#include <memory>
#include <string>
#include <variant>
#include <vector>
#include "ast.hpp"

struct Array;

struct Function {
  FunctionStatement *declaration; // closure can be added later
};

using Value =
    std::variant<int, bool, double, std::string, std::shared_ptr<Array>, std::shared_ptr<Function>>;

struct Array {
  std::vector<Value> elements;
};

struct Variable {
  Value value;
  bool mutable_;
};