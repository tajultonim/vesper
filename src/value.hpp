#pragma once

#include <variant>
#include <string>

using Value = std::variant<int, bool, double, std::string>;

struct Variable {
  Value value;
  bool mutable_;
};