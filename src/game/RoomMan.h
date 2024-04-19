#pragma once
#include <game/Room.h>

namespace sky
{
	namespace lvl
	{
		namespace man
		{
			void init(sf::RenderTarget& target);
			void step(float deltaTime);
			void draw(sf::RenderTarget& target);

			bool changeRoom(std::size_t newRoom);

			// only use if you must
			const room& getCurrentRoom();
		}
	}
}
