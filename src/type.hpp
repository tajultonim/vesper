#pragma once

#include <memory>

struct Type
{
    enum class Kind
    {
        INT,
        FLOAT,
        BOOL,
        STRING,
        ARRAY
    };

    Kind kind;

    std::unique_ptr<Type> elementType;

    Type(Kind kind)
        : kind(kind)
    {
    }

    Type(Kind kind, std::unique_ptr<Type> elementType)
        : kind(kind),
          elementType(std::move(elementType))
    {
    }

    Type(const Type& other)
        : kind(other.kind)
    {
        if (other.elementType)
        {
            elementType =
                std::make_unique<Type>(*other.elementType);
        }
    }

    Type& operator=(const Type& other)
    {
        if (this == &other)
            return *this;

        kind = other.kind;

        if (other.elementType)
        {
            elementType =
                std::make_unique<Type>(*other.elementType);
        }
        else
        {
            elementType.reset();
        }

        return *this;
    }

    Type(Type&&) noexcept = default;

    Type& operator=(Type&&) noexcept = default;
};