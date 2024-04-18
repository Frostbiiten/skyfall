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
			int id; // represents index in room array
			int width, height;

			tilemap map;

			void init();
			void update();
			void draw();

		};
	}
}
