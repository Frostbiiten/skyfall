#pragma once
#include <SFML/Graphics.hpp>

namespace sky
{
	// TODO: remove class, create player-specific components
	class player
	{
		sf::Vector2f position;
		sf::Vector2f dimensions{ 12, 24 };
		float baseSpeed = 10.f;
		sf::RectangleShape debugRect;

	public:
		player();
		void start();
		void update(float deltaTime);
		void draw(sf::RenderTarget& target);
	};
}