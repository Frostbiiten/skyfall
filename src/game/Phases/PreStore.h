#pragma once
#include <game/Room.h>

namespace sky
{
	namespace ecs
	{
		struct prestore
		{
			prestore(flecs::world& world)
			{
				auto moveCamera = world.system<com::player, com::position, com::velocity&>("Move Camera").kind(flecs::PreStore)
					.each([&](flecs::iter& it, size_t, com::player player, com::position p, com::velocity v)
				{
					float targetX = p.x + std::clamp(v.x * player.cameraLookahead, -player.maxCameraLookahead, player.maxCameraLookahead);
					float targetY = p.y + std::clamp(v.y * player.cameraLookahead, -player.maxCameraLookahead, player.maxCameraLookahead);

					sf::Vector2f camPos = cam::getCenter();

					float xDiff = targetX - camPos.x;
					if (xDiff > player.cameraRect.x) camPos.x += (xDiff - player.cameraRect.x);
					if (xDiff < -player.cameraRect.x) camPos.x += (xDiff + player.cameraRect.x);

					float yDiff = targetY - camPos.y;
					if (yDiff > player.cameraRect.y) camPos.y += (yDiff - player.cameraRect.y);
					if (yDiff < -player.cameraRect.y) camPos.y += (yDiff + player.cameraRect.y);

					cam::lerpCenter(camPos, 10.5f, it.delta_time());
				});

				auto clampCamera = world.system<lvl::room>("Room Clamp Camera").kind(flecs::PreStore)
					.each([&](lvl::room room)
				{
					float cameraX = std::max(std::min(cam::getCenter().x, room.width * 16 + room.cameraPadding.x), -room.cameraPadding.x);
					float cameraY = std::max(std::min(cam::getCenter().y, room.height * 16 + room.cameraPadding.y), -room.cameraPadding.y);
					cam::setCenter(sf::Vector2f(cameraX, cameraY));
				});
			}
		};
	}
}
