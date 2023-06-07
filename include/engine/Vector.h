#pragma once

#include <cstdint>

struct Vector2F
{
    float X;
    float Y;

    Vector2F operator +(Vector2F other) const
    {
        return { X + other.X, Y + other.Y };
    }

    Vector2F operator -(Vector2F other) const
    {
        return { X - other.X, Y - other.Y };
    }
};

struct Vector2I
{
    int X;
    int Y;

    explicit operator Vector2F() const
    {
        return { (float)X, (float)Y };
    }

    Vector2I operator +(Vector2I other) const
    {
        return { X + other.X, Y + other.Y };
    }

    Vector2I operator -(Vector2I other) const
    {
        return { X - other.X, Y - other.Y };
    }
};