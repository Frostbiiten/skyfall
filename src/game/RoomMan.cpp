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
			// TODO: ensure this pointer is stable
			sf::RenderTarget* renderTarget;

			// ecs world
			flecs::world world;
			constexpr bool monitorWorld = true;

			// TODO: ensure this works/find another solution with multithreading...
			sf::RectangleShape rectBoundsSurrogate;

			// Register components and systems in a module
			struct ecsRegistry
			{
				ecsRegistry(flecs::world& world)
				{
					// Register components
					world.component<cmp::position>()
						.member<float>("x")
						.member<float>("y");

					world.component<cmp::velocity>()
						.member<float>("x")
						.member<float>("y");

					world.component<cmp::rectBounds>()
						.member<float>("width")
						.member<float>("height")
						.member<float>("xOffset")
						.member<float>("yOffset");

					world.component<sf::Color>()
						.member<sf::Uint8>("r")
						.member<sf::Uint8>("g")
						.member<sf::Uint8>("b")
						.member<sf::Uint8>("a");

					world.component<cmp::drawBounds>()
						.member<sf::Color>("color")
						.member<int>("thickness");

					// Systems

					auto move = world.system<cmp::position, const cmp::velocity>("Move").kind(flecs::OnUpdate)
						.each([](flecs::entity e, cmp::position& p, const cmp::velocity& v) {
						p.x += v.x;
						p.y += v.y;
						// dbg::log()->info("{} moved to ({}, {})", e.path().c_str(), p.x, p.y);
						// dbg::log()->info("{} moved to ({}, {})", e.path().c_str(), p.x, p.y);
					});
					
					rectBoundsSurrogate.setFillColor(sf::Color::Transparent);
					auto drawRectBounds = world.system<const cmp::drawBounds, const cmp::rectBounds, const cmp::position>("DrawRectBounds").kind(flecs::PostUpdate)
						.each([](flecs::entity e, const cmp::drawBounds& b, const cmp::rectBounds rb, const cmp::position& p)
					{
						rectBoundsSurrogate.setOutlineColor(b.color);
						rectBoundsSurrogate.setPosition(p.x + rb.xOffset, p.y + rb.yOffset);
						rectBoundsSurrogate.setSize(sf::Vector2f(rb.width, rb.height));
						rectBoundsSurrogate.setOutlineThickness(b.thickness);

						renderTarget->draw(rectBoundsSurrogate);
						//dbg::log()->info("I should see a rectangle at ({}, {})", p.x, p.y);
					});
				}
			};

			// player
			flecs::entity player;

			// room generation
			constexpr std::size_t reservedRooms = 256;
			std::size_t currentRoom = -1;
			std::vector<room> rooms;

			void init(sf::RenderTarget& target)
			{
				renderTarget = &target;

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

				// Register ecs components/systems
				world.import<ecsRegistry>();

				// Create entity in first world
				// Should I use a prefab ??
				player = world.entity("player").set([]
				(
					cmp::position& p,
					cmp::velocity& v,
					cmp::rectBounds& bounds,
					cmp::drawBounds& db
				) {
					p = { rooms[currentRoom].getWidth() / 2.f, rooms[currentRoom].getHeight() / 2.f };
					v = { 0, 0 };
					bounds = { .width{12}, .height{24}, .xOffset{-6}, .yOffset{-6} };
					db = { .color{sf::Color::Red}, .thickness = 1};
				});

				// Serialize world to JSON (test)
				auto json = world.to_json();
				sky::dbg::log()->info(json.c_str());
			}

			void step(float deltaTime)
			{
				world.progress();

				// TODO: transfer camera ownership to player
				sf::Vector2f newCamCenter(rooms[currentRoom].getWidth() / 2.f, rooms[currentRoom].getHeight() / 2.f);
				sky::cam::lerpCenter(newCamCenter, 10.f, deltaTime);
			}

			void draw(sf::RenderTarget& target)
			{
				// rooms[currentRoom].draw(target);
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
