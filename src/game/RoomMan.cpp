// std
#include <vector>

// rand
#include <effolkronium/random.hpp>
using random = effolkronium::random_static;

// imgui
#include <imgui.h>

// Flecs
#include <flecs.h>

// SKY
#include <sky/Camera.h>
#include <sky/Components.h>
#include <sky/ResourceMan.h>
#include <sky/Tilemap.h>
#include <sky/Input.h>
#include <sky/Debug.h>

// GAME
#include <game/TypeRegistry.h>
#include <game/RoomMan.h>
#include <game/Player.h>
#include <game/Room.h>

// PHASES
#include <game/Phases/Start.h>
#include <game/Phases/OnLoad.h>
#include <game/Phases/PostLoad.h>
#include <game/Phases/PreUpdate.h>
#include <game/Phases/OnUpdate.h>
#include <game/Phases/OnValidate.h>
#include <game/Phases/PostUpdate.h>
#include <game/Phases/PreStore.h>
#include <game/Phases/OnStore.h>

namespace sky
{
	namespace man
	{
		// TODO: ensure this pointer is stable
		// ecs world
		flecs::world world;
		constexpr bool monitorWorld = true;

		// player
		flecs::entity player;

		// Register components and systems in a module
		struct ecsRegistry
		{
			// Pipeline elements

			ecsRegistry(flecs::world & world)
			{
				// register types/components
				world.import<ecs::typeRegistry>();

				// Pipeline
				world.import<ecs::start>();
				world.import<ecs::onload>();
				world.import<ecs::postload>();
				world.import<ecs::preupdate>();
				world.import<ecs::onupdate>();
				world.import<ecs::onvalidate>();
				world.import<ecs::postupdate>();
				world.import<ecs::prestore>();
				world.import<ecs::onstore>();
			}
		};

		void init(sf::RenderTarget& target)
		{
			ecs::renderTarget = &target;

			// Monitor ecs statistics.
			// See https://github.com/flecs-hub/explorer
			if (monitorWorld)
			{
				world.set<flecs::Rest>({});
				world.import<flecs::monitor>();
			}

			// Register ecs components/systems
			world.import<ecsRegistry>();

			int roomWidth = random::get(18, 24);
			int roomHeight = random::get(9, 15);

			// Create initial room
			/**/
			auto currentRoom = world.entity("Current Room").set([&](lvl::room & room)
			{
				room.width = roomWidth;
				room.height = roomHeight;
			}).add<com::loading>();
			//*/

			/**/
			player = world.entity("player").set([&]
			(
				com::position& p,
				com::velocity& v,
				com::rectBounds& bounds,
				com::drawBounds & db
				)
			{
				p = { roomWidth * 8.f, roomHeight * 8.f };
				v = { 30, 20 };
				bounds = { .width{12}, .height{24}, .xOffset{-6}, .yOffset{-6} };
				db = { .color{sf::Color::Red}, .thickness = 1 };
			}).add<com::loading>().add<com::player>();
			//*/
		}

		void step(float deltaTime)
		{
			world.progress(deltaTime);

			ImGui::Begin("Debug");

			if (ImGui::Button("Dump Scene"))
			{
				flecs::string json = world.to_json();
				file::writeFile("dump.json", json.c_str());
				dbg::log()->info(json.c_str());
			}

			if (ImGui::Button("Load Scene"))
			{
				world = flecs::world{};
				if (monitorWorld)
				{
					world.set<flecs::Rest>({});
					world.import<flecs::monitor>();
				}
				world.import<ecsRegistry>();

				std::string sceneData;
				file::readFile("dump.json", sceneData);
				dbg::log()->info(sceneData);

				// not expected to work
				world.from_json(sceneData.data());
			}

			ImGui::End();
		}
	}
}
