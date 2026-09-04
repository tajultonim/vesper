#pragma once

#include <variant>

using Value = std::variant<int, bool, double>;

struct Variable {
  Value value;
  bool mutable_;
};