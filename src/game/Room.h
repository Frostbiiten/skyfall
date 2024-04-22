#pragma once
#include <sky/Tilemap.h>
#include <flecs.h>

// rand
#include <random>

namespace sky
{
	namespace lvl
	{
		struct room
		{
			// rooms do not directly store a "global position", transitions between rooms occur
			std::size_t id; // represents index in room array
			std::size_t width, height;
			sf::Vector2f cameraPadding { -50.f, -50.f };
			flecs::entity tilemap;
		};

		void generateRoom(const room& r, tilemap& tilemap, const tileset& tileset);
	}
}
