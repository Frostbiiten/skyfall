#include <game/room.h>

namespace sky
{
	namespace lvl
	{
		// TODO: update 16x16 etc with variable tile width/height

		room::room(std::size_t id, std::size_t width, std::size_t height) :
			id{ id },
			width { width },
			height{ height },
			map{ 16, 16, width, height, 20 }
		{
			map.load();
		}

		void room::step()
		{

		}

		void room::draw(sf::RenderTarget& target)
		{
			sky::lvl::tile t;
			target.draw(map);
		}

		std::size_t room::getWidth()
		{
			return width * 16;
		}

		std::size_t room::getHeight()
		{
			return height * 16;
		}
	}
}