#pragma once
#include <SFML/Graphics.hpp>

class Game
{
public:
	Game();
	~Game();

	void run();

private:
	void update();
	void render();
	void pollSFMLEvent();

	sf::RenderWindow window;
	sf::Color clearColor;
};