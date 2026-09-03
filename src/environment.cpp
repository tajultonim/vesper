#include "environment.hpp"

void Environment::define(const std::string& name, int value)
{
    variables[name] = value;
}

int Environment::get(const std::string& name) const
{
    return variables.at(name);
}