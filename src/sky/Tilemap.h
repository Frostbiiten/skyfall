#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include <flecs.h>

namespace sky
{
	namespace lvl
	{
		struct tile
		{
			// offset is counter on a row-major basis
			std::size_t offset;
		};

		struct tilemapRenderer
		{

		};

		// Tilesets contain a defined set of tiles in no particular arrangement
		struct tileset
		{
			std::string name;
			std::vector<tile> definitions;
			std::size_t tileWidth, tileHeight;
			std::size_t textureId = 0;
		};

		struct tilemap
		{
			// layout
			std::vector<std::size_t> mapTiles;

			// properties
			std::size_t mapWidth, mapHeight;

			// drawing
			std::vector<sf::Vertex> verts;

			// use vertex buffer when tilemap remains unchanged over frames
			// saves time by storing data on gpu instead of copying every frame
			bool buffered = false;
			sf::VertexBuffer buf;

			// tileset ref
			flecs::entity tileset;
		};

		bool registerTileset(tileset& ts);

		// Registers a tilemap for drawing (similiar to init)
		bool registerTilemap(tilemap& tm, const tileset& ts);
	}
}
