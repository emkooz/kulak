#include "game.hpp"

Game::Game() :
	window(sf::VideoMode(800, 600), "kulak"), // shift to config file later
	clearColor(100, 149, 237, 255),
	world(&window)
{
	loadTextures();
	world.createEntities(world.events);
}

Game::~Game()
{

}

void Game::loadTextures()
{
	kk::log("Loading textures...");
	kk::loadTexture("rtz", "rtzw.jpg");
	kk::loadTexture("player", "player_sheet_fix.png");
	kk::loadTexture("ak", "ak47.png");
	kk::loadTexture("bg", "bg.png");
}

void Game::update(sf::Time deltaTime)
{
	//world.systems.update_all(deltaTime.asSeconds());
	world.update(deltaTime);
}

void Game::render()
{
	std::vector<std::vector<entityx::Entity>> renderList;
	renderList.resize(5); // 5 render layers

	// loop through render layer, render layer should probably be outside of renderable component
	world.entities.each<cRenderable>([this, &renderList](entityx::Entity entity, cRenderable &sprite)
	{
		if (sprite.render) // set to its own component?? as a flag kinda, so this if isn't required, less traversal through entities
		{
			/*sf::RectangleShape debug;
			debug.setFillColor(sf::Color::Transparent);
			debug.setSize(sf::Vector2f(sprite.box->getLocalBounds().width * sprite.box->getScale().x, sprite.box->getLocalBounds().height * sprite.box->getScale().y));
			debug.setOrigin(sf::Vector2f(debug.getSize().x / 2, debug.getSize().y / 2));
			debug.setPosition(sf::Vector2f(sprite.box->getGlobalBounds().left + ((sprite.box->getLocalBounds().width * fabs(sprite.box->getScale().x)) / 2), sprite.box->getGlobalBounds().top + ((sprite.box->getLocalBounds().height * fabs(sprite.box->getScale().y)) / 2)));
			debug.setOutlineThickness(2);
			debug.setOutlineColor(sf::Color::White);
			window.draw(debug);*/
			//window.draw(*sprite.box);
			renderList[sprite.renderLayer].push_back(entity);
		}
	});

	for (int x = 0; x < renderList.size(); x++) // loop through each layer
	{
		for (int y = 0; y < renderList[x].size(); y++) // loop through each item in layer
		{
			window.draw(*renderList[x][y].component<cRenderable>()->box);
		}
	}

	// temporary, later will be a cRenderable
	world.entities.each<cBasicRail>([this](entityx::Entity entity, cBasicRail& rail)
	{
		if (rail.render)
			window.draw(*rail.box);
	});
}

void Game::run()
{
	sf::Time tickrate = sf::seconds(1.0f / 100.0f); // TODO: set as modifiable cvar
	sf::Clock clock;
	sf::Time updateTime = sf::Time::Zero;

	while (kk::getState() != kk::STATE_QUIT) // main loop
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

	window.close();
}

void Game::pollSFMLEvent()
{
	sf::Event event;

	while (window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			world.events.emit<evQuit>();
			break;
		case sf::Event::KeyPressed:
			world.events.emit<evKeyboard>(event.key.code, true);
			break;
		case sf::Event::KeyReleased:
			world.events.emit<evKeyboard>(event.key.code, false);
		}
	}
}