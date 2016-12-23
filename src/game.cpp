#include "game.hpp"

Game::Game() :
	window(sf::VideoMode(800, 600), "kulak"), // shift to config file later
	clearColor(100, 149, 237, 255),
	world(&window)
{

}

Game::~Game()
{

}

void Game::update(sf::Time deltaTime)
{
	world.systems.update_all(deltaTime.asSeconds());
}

void Game::render()
{

}

void Game::run()
{
	sf::Time tickrate = sf::seconds(1.0f / 100.0f); // TODO: set as modifiable cvar
	sf::Clock clock;
	sf::Time updateTime = sf::Time::Zero;

	while (window.isOpen()) // main loop
	{
		sf::Time deltaTime = clock.restart();
		updateTime += deltaTime;
		
		while (updateTime > tickrate)
		{
			updateTime -= tickrate;

			pollSFMLEvent();
			update(tickrate);
		} // possibly do something if update time was lower than tickrate?

		window.clear(clearColor);
		render();
		window.display();
	}
}

void Game::pollSFMLEvent()
{
	sf::Event event;

	while (window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			window.close();
			break;
		case sf::Event::KeyPressed:
			world.events.emit<evKeyboard>(event.key.code, true);
			break;
		case sf::Event::KeyReleased:
			world.events.emit<evKeyboard>(event.key.code, false);
		}
	}
}