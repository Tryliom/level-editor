#include "engine/Utility.h"

#include <cmath>
#include <random>

namespace Utility
{
	float GetDistance(Vector2F a, Vector2F b)
	{
		return std::sqrt(std::pow(b.X - a.X, 2) + std::pow(b.Y - a.Y, 2));
	}

	float GetAngle(Vector2F a, Vector2F b)
	{
		return ToDegrees(std::atan2(b.Y - a.Y, b.X - a.X));
	}

	Vector2F GetDirection(Vector2F a, Vector2F b)
	{
		Vector2F direction = { b.X - a.X, b.Y - a.Y };

        float magnitude = std::sqrt(direction.X * direction.X + direction.Y * direction.Y);

        return { direction.X / magnitude, direction.Y / magnitude };
	}

    Vector2F GetDirection(float angle)
    {
        return { std::cos(ToRadians(angle)), std::sin(ToRadians(angle)) };
    }

	float Range(float min, float max)
	{
		std::random_device os_seed;
		uint_least32_t seed = os_seed();

		float offset = 0;

		if (min < 0)
		{
			max -= min;
			offset = min;
			min = 0;
		}

		std::mt19937 generator(seed);
		std::uniform_real_distribution<float> distribute(min, max);

		return distribute(generator) + offset;
	}

	int Range(int min, int max)
	{
		std::random_device os_seed;
		uint_least32_t seed = os_seed();

		int offset = 0;

		if (min < 0)
		{
			max -= min;
			offset = min;
			min = 0;
		}

		std::mt19937 generator(seed);
		std::uniform_int_distribution<uint_least32_t> distribute(min, max);

		return (int) distribute(generator) + offset;
	}

    float ToRadians(float degrees)
    {
        return degrees * 3.14159265f / 180.f;
    }

    float ToDegrees(float radians)
    {
        return radians * 180.f / 3.14159265f;
    }

	int ToColor(int r, int g, int b, int a)
	{
		// 0 to 255
		r = std::max(0, std::min(255, r));
		g = std::max(0, std::min(255, g));
		b = std::max(0, std::min(255, b));
		a = std::max(0, std::min(255, a));

		return (a << 24) | (r << 16) | (g << 8) | b;
	}
}
