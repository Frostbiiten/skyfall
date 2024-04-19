#pragma once
#include <SFML/Graphics.hpp>

namespace sky
{
	namespace ecs
	{
		// COMPONENTS SHOULD ONLY BE DATA

		struct position
		{
			sf::Vector2f vec;
		};

		struct velocity
		{
			float x;
			float y;
		};

		struct boxBounds
		{
			sf::Vector2f dimensions;
			sf::Vector2f offset;
		};

		struct drawbox { sf::Color color; };
	}
}