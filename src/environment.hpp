#pragma once

#include <string>
#include <unordered_map>

class Environment
{
public:
    void define(const std::string& name, int value);
    int get(const std::string& name, int line, int column) const;

private:
    std::unordered_map<std::string, int> variables;
};