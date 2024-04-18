#pragma once

// SFML
#include <SFML/Graphics.hpp>

namespace sky
{
	namespace render
	{
		constexpr int pixelWidth = 424;
		constexpr int pixelHeight = 240;
		constexpr int scaleFactor = 4;
		constexpr int aaLevel = 0; // set to 8x or 16x for aa

		// Set & Get Camera Position
		void setViewPosition(sf::Vector2f position);

		sf::Vector2f getViewPosition();
	}

	void init();
}
