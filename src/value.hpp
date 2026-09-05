#pragma once

#include <memory>
#include <string>
#include <variant>
#include <vector>

struct Array;

using Value =
    std::variant<int, bool, double, std::string, std::shared_ptr<Array>>;

struct Array {
  std::vector<Value> elements;
};

struct Variable {
  Value value;
  bool mutable_;
};