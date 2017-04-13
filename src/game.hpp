#pragma once
#include <SFML/Graphics.hpp>
#include <entityx/entityx.h>
#include "systems/input.hpp"
#include "log.hpp"
#include "resource.hpp"
#include "cvar.hpp"
#include "world.hpp"
#include <vector>

class Game
{
public:
	Game();
	~Game();

	void run();

private:
	void loadTextures();
	void loadCvars();
	void update(sf::Time deltaTime);
	void render();
	void pollSFMLEvent();

	World world;

	sf::RenderWindow window;
	sf::ContextSettings windowSettings;
	sf::Color clearColor;
};