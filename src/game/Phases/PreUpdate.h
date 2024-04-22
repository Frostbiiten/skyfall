#pragma once

#include <flecs.h>
#include <sky/Components.h>

namespace sky
{
	namespace ecs
	{
		struct preupdate
		{
			preupdate(flecs::world& world)
			{
				// runs once at the beginning of a world.
				auto updatePlayerVelocity = world.system<com::player, com::position, com::velocity&>("Update Player Velocity").kind(flecs::PreUpdate)
					.each([&](flecs::entity, com::player player, com::position p, com::velocity& v)
				{
					v.x = in::getAxis(in::axis::horizontal) * player.speed;
					v.y = -in::getAxis(in::axis::vertical) * player.speed;
				});
			}
		};
	}
}
