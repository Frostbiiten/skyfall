#pragma once
#include <SFML/Graphics.hpp>

namespace sky
{
	namespace ecs
	{
		// COMPONENTS SHOULD ONLY BE DATA

		struct position
		{
			float x;
			float y;
		};

		struct velocity
		{
			float x;
			float y;
		};

		struct boxBounds
		{
			float width;
			float height;
			float xOffset;
			float yOffset;
		};

		struct drawbox { sf::Color color; };
	}
}