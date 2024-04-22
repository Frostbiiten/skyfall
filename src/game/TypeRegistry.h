#pragma once

// flecs
#include <flecs.h>

// SKY
#include <sky/Components.h>

// GAME
#include <game/Player.h>
#include <game/Room.h>

namespace sky
{
	namespace ecs
	{
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
		struct typeRegistry
		{
			typeRegistry(flecs::world & world)
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
	}
}