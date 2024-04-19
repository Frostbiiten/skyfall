#pragma once
#include <sky/Tilemap.h>
#include <flecs.h>

namespace sky
{
	namespace lvl
	{
		class room
		{
			// rooms do not sore a direct "global position", transitions between rooms occur
			std::size_t id; // represents index in room array
			std::size_t width, height;

			tilemap map;

		public:
			room(std::size_t id, std::size_t width, std::size_t height);
			void step();
			void draw(sf::RenderTarget& target);

			std::size_t getWidth();
			std::size_t getHeight();
		};
	}
}
