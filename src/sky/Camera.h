#pragma once
#include <SFML/System/Vector2.hpp>

namespace sky
{
	namespace cam
	{
		void setCenter(sf::Vector2f center);
		sf::Vector2f getCenter();

		// NOTE: Although this lerp is technically inaccurate based on timeframe, the differences are negligible (?)
		// DOUBLE CHECK ON THIS WHEN THE CAMERA STARTS MOVING AROUND
		void lerpCenter(sf::Vector2f center, float step, float deltaTime);
	}
}
