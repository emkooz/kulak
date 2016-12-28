#include "world.hpp"

World::World(sf::RenderWindow* _window)
{
	window = _window;
	configure(events); // configure base before all else
	// generate all systems
	systems.add<inputSystem>(events);
	systems.add<movementSystem>(entities);
	systems.configure();
}

void World::update(sf::Time deltaTime)
{
	// send updates to all systems eg. systems.update<movementsystem>(deltaTime);
	systems.update<inputSystem>(deltaTime.asSeconds());
	systems.update<movementSystem>(deltaTime.asSeconds());
}

void World::createEntities()
{
	ePlayer = entities.create(); // does scope matter? could declare it in this function instead of class?
	ePlayer.assign<health>(100);
	ePlayer.assign<playerID>(0);
	ePlayer.assign<position>(sf::Vector2f(0.f, 0.f));
	ePlayer.assign<direction>(sf::Vector2f(0.f, 0.f));
	ePlayer.assign<velocity>(0.f, 0.f);
	std::unique_ptr<sf::Sprite> pSprite(new sf::Sprite());
	pSprite->setTexture(*kk::getTexture("rtz"));
	ePlayer.assign<renderable>(
		std::move(pSprite),
		0,
		true
		);
}

void World::configure(entityx::EventManager &event_manager)
{
	event_manager.subscribe<evPause>(*this);
	event_manager.subscribe<evResume>(*this);
	event_manager.subscribe<evQuit>(*this);
}

void World::receive(const evPause& pause)
{
	kk::log("Received a pause event.");
}

void World::receive(const evResume& resume)
{
	kk::log("Received a resume event.");
}

void World::receive(const evQuit& quit)
{
	// handle saving
	kk::log("Received a quit event.");
	kk::setState(kk::gameState::STATE_QUIT);
}
