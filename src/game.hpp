#pragma once
#include <SFML/Graphics.hpp>
#include "systems/input.hpp"
#include "world.hpp"

class Game
{
public:
	Game();
	~Game();

	void run();

private:
	void update(sf::Time deltaTime);
	void render();
	void pollSFMLEvent();

	World world;

	sf::RenderWindow window;
	sf::Color clearColor;
};