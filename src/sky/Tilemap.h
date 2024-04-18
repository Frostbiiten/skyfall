#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

namespace sky
{
	namespace lvl
	{
		struct tile
		{
			int offset;

			// perhaps add flags
		};

		class tilemap : public sf::Drawable, public sf::Transformable
		{
			// data
			std::vector<tile> tileDefs;
			std::vector<std::size_t> mapTiles;
			
			// properties
			std::size_t tileWidth, tileHeight;
			std::size_t mapWidth, mapHeight;

			// drawing
			sf::Texture tileset;
			sf::VertexArray verts;

			virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		public:
			tilemap(std::size_t tileWidth, std::size_t tileHeight, std::size_t mapWidth, std::size_t mapHeight, std::size_t uniqueTiles)
				: tileWidth(tileWidth), tileHeight(tileHeight),
				mapWidth(mapWidth), mapHeight(mapHeight)
			{
				mapTiles.resize(mapWidth * mapHeight);
				tileDefs.resize(uniqueTiles);

				for (std::size_t i = 0; i < tileDefs.size(); ++i)
				{
					tileDefs[i].offset = i;
				}
			}

			bool load();
		};
	}
}
