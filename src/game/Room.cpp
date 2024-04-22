#include <game/room.h>


namespace sky
{
	namespace lvl
	{
		// TODO: update 16x16 etc with variable tile width/height
		void generateRoom(const room& r, tilemap& tilemap, const tileset& tileset)
		{
			tilemap.mapWidth = r.width;
			tilemap.mapHeight = r.height;
			tilemap.buffered = true;
			registerTilemap(tilemap, tileset);

			// write tiles
			std::size_t currentTile = 0;
			for (int y = 0; y < tilemap.mapHeight; ++y)
			{
				for (int x = 0; x < tilemap.mapWidth; ++x)
				{
					int intendedTile = 3;
					if (x == 0 && y == 0) intendedTile = 0;
					else if (y == 0) intendedTile = 1;
					else if (y == tilemap.mapWidth - 1) intendedTile = 2;
					tilemap.mapTiles[currentTile] = intendedTile;
					++currentTile;
				}
			}

			// mesh regen must be called manually.
		}
	}
}