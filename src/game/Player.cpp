#include "Player.h"
namespace sky
{
	player::player()
	{
		debugRect = sf::RectangleShape { dimensions };
		debugRect.setFillColor(sf::Color::Transparent);
		debugRect.setOutlineColor(sf::Color::Red);
		debugRect.setOutlineThickness(1);
	}

	void player::start()
	{
	}

	void player::update(float deltaTime)
	{
	}

	void player::draw(sf::RenderTarget& target)
	{
		//debugRect.setPosition(position);
		//target.draw(debugRect);
	}
}