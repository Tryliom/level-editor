#pragma once

#include <cstdint>
#include <string>

#include "Vector.h"

enum Color
{
    Black = 0xFF000000,
    White = 0xFFFFFFFF,
    Red = 0xFFFF0000,
    Green = 0xFF00FF00,
    Blue = 0xFF0000FF
};

enum Pivot
{
    TopLeft,
    Center,
	TopRight,
};

struct Text
{
    std::string Text;
    float Size { 32.f };
    Vector2I Position { 0, 0 };
	uint32_t Color { 0xFFFFFFFF };
    Pivot Pivot { Pivot::TopLeft };
    bool Shadow { true };
};

namespace Utility
{
	float GetDistance(Vector2F a, Vector2F b);
	float GetAngle(Vector2F a, Vector2F b);
	Vector2F GetDirection(Vector2F a, Vector2F b);
    Vector2F GetDirection(float angle);
	float Range(float min, float max);
	int Range(int min, int max);
    float ToRadians(float degrees);
    float ToDegrees(float radians);
	int ToColor(int r, int g, int b, int a = 255);
}