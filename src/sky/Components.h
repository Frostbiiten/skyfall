#pragma once
#include <SFML/Graphics.hpp>

namespace sky
{
	namespace com
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

		struct teleport
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

		struct drawBounds { sf::Color color; float thickness = 1.f; };
		struct loading {};


		// Game
		struct dmg { float dmg; };
	}
}