#pragma once
#include <SFML/Graphics.hpp>
#include <entityx/entityx.h>
#include "systems/input.hpp"
#include "log.hpp"
#include "resource.hpp"
#include "world.hpp"

class Game
{
public:
	Game();
	~Game();

	void run();

private:
	void loadTextures();
	void update(sf::Time deltaTime);
	void render();
	void pollSFMLEvent();

	World world;

	sf::RenderWindow window;
	sf::Color clearColor;
};