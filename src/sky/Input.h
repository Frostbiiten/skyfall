#pragma once
#include <SFML/Graphics.hpp>

namespace sky 
{
	namespace in
	{
		constexpr enum axis
		{
			horizontal = 0,
			vertical = 1,
		};

		void init();
		void step(sf::RenderWindow* windowPtr, int scaleFactor = 1);
		void imgui();
		void processEvent(sf::Event event);

		float getAxis(int axis);
		sf::Vector2i getPointerPosition();
	}
}