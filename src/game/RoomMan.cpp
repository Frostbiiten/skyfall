// std
#include <vector>

// Flecs
#include <flecs.h>

// SKY
#include <sky/Camera.h>
#include <sky/Components.h>
#include <sky/ResourceMan.h>
#include <sky/Debug.h>

// GAME
#include <game/RoomMan.h>
#include <game/Player.h>
#include <sky/Tilemap.h>
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
						.member<int>("thickness");

					world.component<lvl::tile>()
						.member(flecs::U64, "offset");

					world.component<std::vector<lvl::tile>>()
						.opaque(std_vector_support<lvl::tile>);

					world.component<lvl::tileset>()
						.member<std::string>("name")
						.member<std::vector<lvl::tile>>("definitions")
						.member(flecs::U64, "tile width")
						.member(flecs::U64, "tile height")
						.member(flecs::U64, "texture Id");

					world.component<std::vector<sf::Vertex>>()
						.opaque(std_vector_support<sf::Vertex>);

					world.component<std::vector<flecs::u64_t>>()
						.opaque(std_vector_support<flecs::u64_t>);

					world.component<lvl::tilemap>()
						.member<std::vector<flecs::u64_t>>("tiles")
						.member(flecs::U64, "map width")
						.member(flecs::U64, "map height")
						.member<std::vector<sf::Vertex>>("vertarray")
						.member<bool>("buffered");
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
					auto loadTilesets = world.system<lvl::tileset, com::loading>("Load Tilesets").kind(flecs::OnLoad)
						.each([](flecs::entity e, lvl::tileset & ts, com::loading l)
					{
						lvl::registerTileset(ts);
						e.remove<com::loading>();
					});

					auto loadTilemaps = world.system<lvl::tilemap, com::loading>("Load Tilemaps").kind(flecs::OnLoad)
						.each([](flecs::entity e, lvl::tilemap & tm, com::loading l)
					{
						lvl::registerTilemap(tm, *tm.tileset.get<lvl::tileset>());
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
					auto moveCamera = world.system<com::player, com::position>("Move Camera").kind(flecs::PreStore)
						.each([&](flecs::iter & it, size_t, com::player, com::position p)
					{
						cam::lerpCenter(sf::Vector2f(p.x, p.y), 3.f, it.delta_time());
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


						ImGui::Begin("Debug");

						if (ImGui::Button("Dump Scene"))
						{
							auto json = world.to_json();
							file::writeFile("dump.json", json.c_str());
							dbg::log()->info(json.c_str());
						}

						ImGui::End();
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

		// player
		flecs::entity player;

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

			// Create entity in first world
			// Should I use a prefab ??
			player = world.entity("player").set([]
			(
				com::position& p,
				com::velocity& v,
				com::rectBounds& bounds,
				com::drawBounds & db
				)
			{
				//p = { rooms[currentRoom].getWidth() / 2.f, rooms[currentRoom].getHeight() / 2.f };
				p = { 0, 0 };
				v = { 30, 20 };
				bounds = { .width{12}, .height{24}, .xOffset{-6}, .yOffset{-6} };
				db = { .color{sf::Color::Red}, .thickness = 1 };
			}).add<com::loading>().add<com::player>();

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
		}

		void step(float deltaTime)
		{
			world.progress(deltaTime);

			// TODO: transfer camera ownership to player
			// sf::Vector2f newCamCenter(rooms[currentRoom].getWidth() / 2.f, rooms[currentRoom].getHeight() / 2.f);
			// sky::cam::lerpCenter(newCamCenter, 10.f, deltaTime);
		}
	}
}
