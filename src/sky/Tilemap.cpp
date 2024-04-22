#include <sky/Tilemap.h>
#include <sky/Debug.h>
#include <sky/ResourceMan.h>

namespace sky
{
	namespace lvl
	{
		bool registerTileset(tileset& ts)
		{
			// TODO: tileset should be read from a file
			dbg::log()->warn("Implement assetman for file tileset loading. [non-default]");
			if ((ts.textureId = res::textures().getId(fmt::format("game/tileset/{}/img.png", ts.name))) == -1) return false;

			int uniqueTiles = 20;
			ts.definitions.resize(uniqueTiles);

			for (std::size_t i = 0; i < ts.definitions.size(); ++i)
			{
				ts.definitions[i].offset = i;
			}

			return true;
		}

		void registerTilemap(tilemap& tm, const tileset& ts)
		{
			tm.mapTiles.resize(tm.mapWidth * tm.mapHeight);

			// resize the vertex array to fit the level size
			std::size_t vertCount = tm.mapWidth * tm.mapHeight * 4;
			tm.verts.resize(vertCount);
		}

		bool regenerateMesh(tilemap& tm, const tileset& ts)
		{
			std::size_t vertCount = tm.mapWidth * tm.mapHeight * 4;
			dbg::log()->info("Loading tilemap: {} vertices", tm.verts.size());

			// quad per tile
			std::size_t currentTile = 0;
			for (std::size_t y = 0; y < tm.mapHeight; ++y)
			{
				for (std::size_t x = 0; x < tm.mapWidth; ++x)
				{
					// get the current tile number
					std::size_t tileID = tm.mapTiles[currentTile];
					tile tileData = ts.definitions[tileID];

					// ptr to current tile quad
					sf::Vertex* quad = &tm.verts[currentTile * 4];

					// quad vert positions
					quad[0].position = sf::Vector2f(x * ts.tileWidth, y * ts.tileHeight);
					quad[1].position = sf::Vector2f((x + 1) * ts.tileWidth, y * ts.tileHeight);
					quad[2].position = sf::Vector2f((x + 1) * ts.tileWidth, (y + 1) * ts.tileHeight);
					quad[3].position = sf::Vector2f(x * ts.tileWidth, (y + 1) * ts.tileHeight);

					// quad uv positions
					std::size_t ty = tileData.offset / tm.mapWidth;
					std::size_t tx = tileData.offset - (ty * tm.mapWidth);

					quad[0].texCoords = sf::Vector2f(tx * ts.tileWidth, ty * ts.tileHeight);
					quad[1].texCoords = sf::Vector2f((tx + 1) * ts.tileWidth, ty * ts.tileHeight);
					quad[2].texCoords = sf::Vector2f((tx + 1) * ts.tileWidth, (ty + 1) * ts.tileHeight);
					quad[3].texCoords = sf::Vector2f(tx * ts.tileWidth, (ty + 1) * ts.tileHeight);

					++currentTile;
				}
			}

			// apply to buffer
			if (tm.buffered)
			{
				tm.buf.create(vertCount);
				tm.buf.setPrimitiveType(sf::Quads);
				tm.buf.update(&tm.verts[0], vertCount, 0);

				/*
				tm.verts.clear();
				std::vector<sf::Vertex> emp;
				tm.verts.swap(emp);
				*/
			}

			return true;
		}
	}
}
