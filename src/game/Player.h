#pragma once
#include <SFML/Graphics.hpp>

namespace sky
{
	namespace com
	{
		struct player
		{
			float speed = 100.f;
			float cameraLookahead = 0.25f;
			float maxCameraLookahead = 140.f;
			sf::Vector2f cameraRect = { 40.f, 20.f };
		};
	}
}