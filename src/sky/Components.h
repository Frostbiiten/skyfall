#pragma once
#include <SFML/Graphics.hpp>

namespace sky
{
	namespace cmp
	{
		// COMPONENTS SHOULD ONLY BE DATA

		struct position
		{
			float x, y;
		};

		struct velocity
		{
			float x, y;
		};

		struct rectBounds
		{
			float width;
			float height;
			float xOffset;
			float yOffset;
		};

		struct drawBounds { sf::Color color; int thickness = 1; };

		// Game
		struct dmg { float dmg; };
	}
}