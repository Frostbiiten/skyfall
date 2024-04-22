#pragma once
#include <flecs.h>
#include <sky/Components.h>
#include <sky/Camera.h>
#include <game/Room.h>

namespace sky
{
	namespace ecs
	{
		struct onload
		{
			onload(flecs::world& world)
			{
				auto loadRooms = world.system<lvl::room, com::loading>("Load Rooms").kind(flecs::OnLoad)
					.each([&](flecs::entity e, lvl::room& room, com::loading)
				{
					// Create basic tileset
					auto ts1 = world.entity("ts1").set([](lvl::tileset& ts)
					{
						ts.name = "proto";
						ts.tileWidth = 16;
						ts.tileHeight = 16;
					}).add<com::loading>();

					// Create tilemap
					auto tm1 = world.entity("tm1").set([&](lvl::tilemap& tm, com::position& p)
					{
						tm.tileset = ts1; // NEED THIS
						lvl::generateRoom(room, tm, *ts1.get<lvl::tileset>());
					}).add<com::loading>();

					// Move camera to center of room
					sf::Vector2f roomCenter { room.width * 8.f, room.height * 8.f };
					cam::setCenter(roomCenter);

					e.remove<com::loading>();
				});

				auto loadTilesets = world.system<lvl::tileset, com::loading>("Load Tilesets").kind(flecs::OnLoad)
					.each([](flecs::entity e, lvl::tileset& ts, com::loading)
				{
					lvl::registerTileset(ts);
					e.remove<com::loading>();
				}).depends_on(loadRooms);

				auto tilemapMeshgen = world.system<lvl::tilemap, com::loading>("Load Tilemaps").kind(flecs::OnLoad)
					.each([](flecs::entity e, lvl::tilemap& tm, com::loading)
				{
					lvl::regenerateMesh(tm, *tm.tileset.get<lvl::tileset>());
					e.remove<com::loading>();
				}).depends_on(loadTilesets);
			}
		};
	}
}
