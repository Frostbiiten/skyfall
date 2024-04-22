#pragma once
#include <flecs.h>
#include <sky/Components.h>

namespace sky
{
	namespace ecs
	{
		struct onvalidate
		{
			onvalidate(flecs::world& world)
			{
				auto move = world.system<com::position, const com::velocity>("Move").kind(flecs::OnUpdate)
					.each([](flecs::iter & it, size_t, com::position & p, const com::velocity & v)
				{
					p.x += v.x * it.delta_time();
					p.y += v.y * it.delta_time();
				});

				auto teleport = world.system<com::position, const com::teleport>("Teleport").kind(flecs::OnUpdate)
					.each([](flecs::entity e, com::position & p, const com::teleport& tp)
				{
					p.x = tp.x;
					p.y = tp.y;
					e.remove<com::teleport>();
				});
			}
		};
	}
}
