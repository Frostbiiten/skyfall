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
			namespace serialization
			{
				// Register components and systems in a module. This excludes them by default
				// from the serialized data, and makes it easier to import across worlds.
				struct move {
					move(flecs::world& world) {
						world.component<ecs::position>()
							.member<float>("x")
							.member<float>("y");

						world.component<ecs::velocity>()
							.member<float>("x")
							.member<float>("y");
					}
				};
			}

			// ecs world
			flecs::world world;
			constexpr bool monitorWorld = true;

			// player
			flecs::entity playerEntity;

			// room generation
			constexpr std::size_t reservedRooms = 256;
			std::size_t currentRoom = -1;
			std::vector<room> rooms;

			void init()
			{
				// Monitor ecs statistics.
				// See https://github.com/flecs-hub/explorer
				if (monitorWorld)
				{
					world.set<flecs::Rest>({});
					world.import<flecs::monitor>();
				}

				world.import<serialization::move>();

				// Create initial rooms
				{
					rooms.reserve(reservedRooms);

					room startRoom { 0, 23, 12 };
					rooms.push_back(std::move(startRoom));
					changeRoom(0);
				}

				// Create entities
				// can this be serialized?
				playerEntity = world.entity("player").set([](ecs::position& p, ecs::velocity& v) {
					p.vec = sf::Vector2f{5, 9};
					v = { 1, 3 };
				});

				// Create systems

				// Systems
				flecs::system sys =
					world.system<ecs::position, const ecs::velocity>("Move")
					.each([](ecs::position& p, const ecs::velocity& v)
				{
					p.vec.x += v.x;
					p.vec.y += v.y;
				});

				// Serialize world to JSON
				auto json = world.to_json();
				sky::dbg::log()->info(json.c_str());
			}

			void update(float deltaTime)
			{
				// TODO: transfer camera ownership to player
				sf::Vector2f newCamCenter(rooms[currentRoom].getWidth() / 2.f, rooms[currentRoom].getHeight() / 2.f);
				sky::cam::lerpCenter(newCamCenter, 10.f, deltaTime);

				world.progress();
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
