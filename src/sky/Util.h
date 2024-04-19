#pragma once
#include <SFML/System/Vector2.hpp>
#include <cmath>

namespace sky
{
	namespace util
	{
		namespace vec
		{
			template <typename T>
			inline sf::Vector2<T> lerp(const sf::Vector2<T>& a, const sf::Vector2<T>& b, float t)
			{
				return a + (b - a) * t;
			}

			template <typename T>
			inline float sqrMagnitude(const sf::Vector2<T>& vector)
			{
				return (vector.x * vector.x) + (vector.y * vector.y);
			}

			template <typename T>
			inline float magnitude(const sf::Vector2<T>& vector)
			{
				return std::sqrtf(sqrMagnitude);
			}
		}
	}
}

