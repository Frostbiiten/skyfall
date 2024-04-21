#pragma once
#include <sky/Tilemap.h>
#include <flecs.h>

namespace sky
{
	namespace lvl
	{
		struct room
		{
			// rooms do not sore a direct "global position", transitions between rooms occur
			std::size_t id; // represents index in room array
			std::size_t width, height;
		};
	}
}
