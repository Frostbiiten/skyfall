// std
#include <vector>

// SKY
#include <sky/Camera.h>
#include <sky/Components.h>
#include <sky/Debug.h>

// GAME
#include <game/RoomMan.h>
#include <game/Player.h>

namespace sky
{
	namespace lvl
	{
		namespace man
		{
			// ecs world
			flecs::world world;
			flecs::world world2Test;
			constexpr bool monitorWorld = true;

			// Register components and systems in a module
			struct ecsRegistry
			{
				ecsRegistry(flecs::world& world)
				{
					// Register components
					world.component<ecs::position>()
						.member<float>("x")
						.member<float>("y");

					world.component<ecs::velocity>()
						.member<float>("x")
						.member<float>("y");

					// Systems
					world.system<ecs::position, const ecs::velocity>("Move")
						.each([](flecs::entity e, ecs::position& p, const ecs::velocity& v) {
						p.x += v.x;
						p.y += v.y;
						dbg::log()->info("{} moved to ({}, {})", e.path().c_str(), p.x, p.y);
					});
				}
			};

			// player
			flecs::entity playerEntity;

			// room generation
			constexpr std::size_t reservedRooms = 256;
			std::size_t currentRoom = -1;
			std::vector<room> rooms;

			void init()
			{
				// Create initial rooms
				{
					rooms.reserve(reservedRooms);

					room startRoom { 0, 23, 12 };
					rooms.push_back(std::move(startRoom));
					changeRoom(0);
				}

				// Monitor ecs statistics.
				// See https://github.com/flecs-hub/explorer
				if (monitorWorld)
				{
					world.set<flecs::Rest>({});
					world.import<flecs::monitor>();
				}

				// Register ecs
				world.import<ecsRegistry>();

				// Create entity in first world
				playerEntity = world.entity("player").set([](ecs::position& p, ecs::velocity& v) {
					p = { 5, 9 };
					v = { 1, 3 };
				});
				world.progress();

				// Create second world, import same module
				flecs::world world_b;
				world_b.import<ecsRegistry>();

				// Serialize world to JSON
				auto json = world.to_json();
				sky::dbg::log()->info(json.c_str());

				// Deserialize JSON into second world
				world_b.from_json(json);
				sky::dbg::log()->info(json.c_str());
			}

			void update(float deltaTime)
			{
				// TODO: transfer camera ownership to player
				sf::Vector2f newCamCenter(rooms[currentRoom].getWidth() / 2.f, rooms[currentRoom].getHeight() / 2.f);
				sky::cam::lerpCenter(newCamCenter, 10.f, deltaTime);

				//world.progress();
			}

			void draw(sf::RenderTarget& target)
			{
				rooms[currentRoom].draw(target);
			}

			bool changeRoom(std::size_t newRoom)
			{
				if (currentRoom == newRoom) return false;
				currentRoom = newRoom;
				return true;
			}

			const room& getCurrentRoom()
			{
				return rooms[currentRoom];
			}
		}
	}
}
