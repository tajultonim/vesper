#pragma once

#include <variant>

using Value = std::variant<int, bool>;

struct Variable {
  Value value;
  bool mutable_;
};