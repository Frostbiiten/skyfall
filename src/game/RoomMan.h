#pragma once
#include <SFML/Graphics.hpp>

namespace sky
{
	namespace man
	{
		void init(sf::RenderTarget& target);
		void step(float deltaTime);
	}
}
