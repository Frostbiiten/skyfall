#pragma once
#include <SFML/Graphics.hpp>

namespace sky
{
	namespace ecs
	{
		inline sf::RenderTarget* renderTarget;
		inline sf::RenderStates rs{};
		inline sf::RectangleShape rectBoundsSurrogate;

		struct onstore
		{
			onstore(flecs::world& world)
			{
				auto drawTilemaps = world.system<com::position, lvl::tilemap>("DrawTilemaps").kind(flecs::OnStore)
					.each([&](flecs::entity e, com::position& p, const lvl::tilemap& tm)
				{
					if (world.is_alive(tm.tileset))
					{
						std::size_t textureId = tm.tileset.get<lvl::tileset>()->textureId;
						rs.texture = &res::textures().getResource(textureId);
						if (tm.buffered)
						{
							renderTarget->draw(tm.buf, rs);
						}
						else
						{
							renderTarget->draw(&tm.verts[0], tm.verts.size(), sf::Quads, rs);
						}
					}

					// temp: attempt reload if no verts
					if (tm.buffered)
					{
						if (tm.buf.getVertexCount() == 0) e.add<com::loading>();
					}
					else if (tm.verts.size() == 0) e.add<com::loading>();
					// dbg::log()->error("Implement resource manager to draw textures properly");
					// rs.texture
				});

				rectBoundsSurrogate.setFillColor(sf::Color::Transparent);
				auto drawRectBounds = world.system<const com::drawBounds, const com::rectBounds, const com::position>("DrawRectBounds").kind(flecs::OnStore)
					.each([&](flecs::entity e, const com::drawBounds& b, const com::rectBounds rb, const com::position& p)
				{
					rectBoundsSurrogate.setOutlineColor(b.color);
					rectBoundsSurrogate.setPosition(p.x + rb.xOffset, p.y + rb.yOffset);
					rectBoundsSurrogate.setSize(sf::Vector2f(rb.width, rb.height));
					rectBoundsSurrogate.setOutlineThickness(b.thickness);

					renderTarget->draw(rectBoundsSurrogate);
				});
			}
		};
	}
}
