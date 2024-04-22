// std
#include <vector>

// rand
#include <effolkronium/random.hpp>
using random = effolkronium::random_static;

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
#include <game/RoomMan.h>
#include <game/Player.h>
#include <game/Room.h>
#include <imgui.h>

namespace sky
{
	namespace man
	{
		// TODO: ensure this pointer is stable
		sf::RenderTarget* renderTarget;
		sf::RenderStates rs{};

		// ecs world
		flecs::world world;
		constexpr bool monitorWorld = true;

		// player
		flecs::entity player;

		// TODO: ensure this works/find another solution with multithreading...
		sf::RectangleShape rectBoundsSurrogate;

		// Reusable reflection support for std::vector
		template <typename Elem, typename Vector = std::vector<Elem>>
		flecs::opaque<Vector, Elem> std_vector_support(flecs::world & world)
		{
			return flecs::opaque<Vector, Elem>().as_type(world.vector<Elem>())

			.serialize([](const flecs::serializer * s, const Vector * data)
			// Forward elements of std::vector value to serializer
			{
				for (const auto & el : *data)
				{
					s->value(el);
				}
				return 0;
			})

			// Return vector count
			.count([](const Vector * data)
			{
				return data->size();
			})

			// Resize contents of vector
			.resize([](Vector * data, size_t size)
			{
				data->resize(size);
			})

			// Ensure element exists, return pointer
			.ensure_element([](Vector * data, size_t elem)
			{
				if (data->size() <= elem)
				{
					data->resize(elem + 1);
				}

				return &data->data()[elem];
			});
		}

		// Register components and systems in a module
		struct ecsRegistry
		{
			struct typeRegistry
			{
				typeRegistry(flecs::world & world)
				{
					world.component<com::position>()
						.member<float>("x")
						.member<float>("y");

					world.component<com::teleport>()
						.member<float>("x")
						.member<float>("y");

					world.component<com::velocity>()
						.member<float>("x")
						.member<float>("y");

					world.component<com::rectBounds>()
						.member<float>("width")
						.member<float>("height")
						.member<float>("xOffset")
						.member<float>("yOffset");

					world.component<sf::Color>()
						.member<sf::Uint8>("r")
						.member<sf::Uint8>("g")
						.member<sf::Uint8>("b")
						.member<sf::Uint8>("a");

					world.component<sf::Vector2f>()
						.member<float>("x")
						.member<float>("y");

					world.component<sf::Vertex>()
						.member<sf::Vector2f>("position")
						.member<sf::Color>("color")
						.member<sf::Vector2f>("texcoords");

					world.component<com::drawBounds>()
						.member<sf::Color>("color")
						.member<float>("thickness");

					world.component<lvl::tile>()
						.member(flecs::U64, "offset");

					world.component<std::vector<lvl::tile>>()
						.opaque(std_vector_support<lvl::tile>);

					world.component<lvl::tileset>()
						.member<std::string>("name")
						.member<std::vector<lvl::tile>>("definitions")
						.member(flecs::U64, "tile_width")
						.member(flecs::U64, "tile_height")
						.member(flecs::U64, "texture_Id");

					world.component<lvl::room>()
						.member(flecs::U64, "id")
						.member(flecs::U64, "width")
						.member(flecs::U64, "height")
						.member<sf::Vector2f>("cameraPadding")
						.member<flecs::entity>("tilemap");

					world.component<std::vector<sf::Vertex>>()
						.opaque(std_vector_support<sf::Vertex>);

					world.component<std::vector<flecs::u64_t>>()
						.opaque(std_vector_support<flecs::u64_t>);

					world.component<lvl::tilemap>()
						.member<std::vector<flecs::u64_t>>("tiles")
						.member(flecs::U64, "map_width")
						.member(flecs::U64, "map_height")
						.member<std::vector<sf::Vertex>>("vertarray")
						.member<bool>("buffered")
						.member<flecs::entity>("tileset");

					world.component<com::player>()
						.member<float>("speed")
						.member<float>("camera_lookahead")
						.member<float>("max_camera_lookahead")
						.member<sf::Vector2f>("camera_rect");
				}
			};

			// Pipeline elements
			struct start
			{
				start(flecs::world & world)
				{
				}
			};
			struct onload
			{
				onload(flecs::world & world)
				{
					auto loadRooms = world.system<lvl::room, com::loading>("Load Rooms").kind(flecs::OnLoad)
						.each([&](flecs::entity e, lvl::room & room, com::loading)
					{
						// Create basic tileset
						auto ts1 = world.entity("ts1").set([](lvl::tileset & ts)
						{
							ts.name = "proto";
							ts.tileWidth = 16;
							ts.tileHeight = 16;
						}).add<com::loading>();

						// Create tilemap
						auto tm1 = world.entity("tm1").set([&](lvl::tilemap & tm, com::position & p)
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
						.each([](flecs::entity e, lvl::tileset & ts, com::loading)
					{
						lvl::registerTileset(ts);
						e.remove<com::loading>();
					}).depends_on(loadRooms);

					auto tilemapMeshgen = world.system<lvl::tilemap, com::loading>("Load Tilemaps").kind(flecs::OnLoad)
						.each([](flecs::entity e, lvl::tilemap & tm, com::loading)
					{
						lvl::regenerateMesh(tm, *tm.tileset.get<lvl::tileset>());
						e.remove<com::loading>();
					}).depends_on(loadTilesets);
				}
			};
			struct postload
			{
				postload(flecs::world & world)
				{
				}
			};
			struct preupdate
			{
				preupdate(flecs::world & world)
				{
					auto updatePlayerVelocity = world.system<com::player, com::position, com::velocity&>("Update Player Velocity").kind(flecs::PreUpdate)
						.each([&](flecs::entity, com::player player, com::position p, com::velocity& v)
					{
						v.x = in::getAxis(in::axis::horizontal) * player.speed;
						v.y = -in::getAxis(in::axis::vertical) * player.speed;
					});
				}
			};
			struct onupdate
			{
				onupdate(flecs::world & world)
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
			struct onvalidate
			{
				onvalidate(flecs::world & world)
				{
				}
			};
			struct postupdate
			{
				postupdate(flecs::world & world)
				{
				}
			};
			struct prestore
			{
				prestore(flecs::world & world)
				{
					auto moveCamera = world.system<com::player, com::position, com::velocity&>("Move Camera").kind(flecs::PreStore)
						.each([&](flecs::iter & it, size_t, com::player player, com::position p, com::velocity v)
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
			struct onstore
			{
				onstore(flecs::world & world)
				{
					auto drawTilemaps = world.system<com::position, lvl::tilemap>("DrawTilemaps").kind(flecs::OnStore)
						.each([&](flecs::entity e, com::position & p, const lvl::tilemap & tm)
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
						else if (tm.verts.size() == 0)
						{
							e.add<com::loading>();
						}
						// dbg::log()->error("Implement resource manager to draw textures properly");
						// rs.texture
					});

					rectBoundsSurrogate.setFillColor(sf::Color::Transparent);
					auto drawRectBounds = world.system<const com::drawBounds, const com::rectBounds, const com::position>("DrawRectBounds").kind(flecs::OnStore)
						.each([&](flecs::entity e, const com::drawBounds & b, const com::rectBounds rb, const com::position & p)
					{
						rectBoundsSurrogate.setOutlineColor(b.color);
						rectBoundsSurrogate.setPosition(p.x + rb.xOffset, p.y + rb.yOffset);
						rectBoundsSurrogate.setSize(sf::Vector2f(rb.width, rb.height));
						rectBoundsSurrogate.setOutlineThickness(b.thickness);

						renderTarget->draw(rectBoundsSurrogate);
					});
				}
			};

			ecsRegistry(flecs::world & world)
			{
				// Register reflection for std::string
				world.component<std::string>().opaque(flecs::String).serialize
				(
					[](const flecs::serializer * s, const std::string * data)
					{
						const char * str = data->c_str();
						return s->value(flecs::String, &str); // Forward to serializer
					}
				)
					.assign_string([](std::string * data, const char * value)
					{
						*data = value; // Assign new value to std::string
					}
				);

				// Register components
				world.import<typeRegistry>();

				// Pipeline
				world.import<start>();
				world.import<onload>();
				world.import<postload>();
				world.import<preupdate>();
				world.import<onupdate>();
				world.import<onvalidate>();
				world.import<postupdate>();
				world.import<prestore>();
				world.import<onstore>();
			}
		};

		void init(sf::RenderTarget& target)
		{
			renderTarget = &target;

			// Monitor ecs statistics.
			// See https://github.com/flecs-hub/explorer
			if (monitorWorld)
			{
				world.set<flecs::Rest>({});
				world.import<flecs::monitor>();
			}

			// Register ecs components/systems
			world.import<ecsRegistry>();

			// Should I use a prefab ??

			/*
			// Create basic tileset
			auto ts1 = world.entity("ts1").set([](lvl::tileset & ts)
			{
				ts.name = "proto";
				ts.tileWidth = 16;
				ts.tileHeight = 16;
			}).add<com::loading>();

			// Create tilemap
			auto tm1 = world.entity("tm1").set([&](lvl::tilemap & tm, com::position & p)
			{
				p = { 0, 0 };
				tm.mapWidth = 200;
				tm.mapHeight = 200;
				tm.tileset = ts1;
				tm.buffered = true;
			}).add<com::loading>();
			//*/

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

		// must be present for persistence 
		// std::string sceneData;
		// flecs::string json;

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

			// TODO: transfer camera ownership to player
			// sf::Vector2f newCamCenter(rooms[currentRoom].getWidth() / 2.f, rooms[currentRoom].getHeight() / 2.f);
			// sky::cam::lerpCenter(newCamCenter, 10.f, deltaTime);
		}
	}
}
