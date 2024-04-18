#include <sky/Tilemap.h>
#include <sky/Debug.h>

namespace sky
{
	namespace lvl
	{
        bool tilemap::load()
		{
            mapWidth = mapHeight = 200;

            // load the tileset texture
            if (!tileset.loadFromFile("prototileset.png")) return false;

            // resize the vertex array to fit the level size
            verts.setPrimitiveType(sf::Quads);
            verts.resize(mapWidth * mapHeight * 4);

            dbg::log()->info("Loading tilemap");
            dbg::log()->info("{}", verts.getVertexCount());

            std::size_t currentTile = 0;

            // quad per tile
			for (std::size_t y = 0; y < mapHeight; ++y)
			{
				for (std::size_t x = 0; x < mapWidth; ++x)
				{
                    mapTiles[currentTile] = currentTile % tileDefs.size();

                    // get the current tile number
                    std::size_t tileID = mapTiles[currentTile];
                    tile tileData = tileDefs[tileID];

                    // ptr to current tile quad
                    sf::Vertex* quad = &verts[currentTile * 4];

                    // quad vert positions
                    quad[0].position = sf::Vector2f(x * tileWidth, y * tileHeight);
                    quad[1].position = sf::Vector2f((x + 1) * tileWidth, y * tileHeight);
                    quad[2].position = sf::Vector2f((x + 1) * tileWidth, (y + 1) * tileHeight);
                    quad[3].position = sf::Vector2f(x * tileWidth, (y + 1) * tileHeight);

                    // quad uv positions
                    std::size_t ty = tileData.offset / mapWidth;
                    std::size_t tx = tileData.offset- (ty * mapWidth);

                    quad[0].texCoords = sf::Vector2f(tx * tileWidth, ty * tileHeight);
                    quad[1].texCoords = sf::Vector2f((tx + 1) * tileWidth, ty * tileHeight);
                    quad[2].texCoords = sf::Vector2f((tx + 1) * tileWidth, (ty + 1) * tileHeight);
                    quad[3].texCoords = sf::Vector2f(tx * tileWidth, (ty + 1) * tileHeight);

                    ++currentTile;
                }
            }

            return true;
		}

		void tilemap::draw(sf::RenderTarget& target, sf::RenderStates states) const
		{
            // TODO: implement culling (draw has start/end for contiguous areas)
            // perhaps split into chunks in non-major axis

			// apply transform
			states.transform *= getTransform();

			// apply tileset texture
			states.texture = &tileset;

			// draw vertex array
			target.draw(verts, states);
		}
	}
}
