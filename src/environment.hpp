#pragma once

#include <string>
#include <unordered_map>

#include "value.hpp"

class Environment
{
public:
    void define(const std::string& name, Value value);
    Value get(const std::string& name, int line, int column) const;

private:
    std::unordered_map<std::string, Value> variables;
};