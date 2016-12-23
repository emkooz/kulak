#include "world.hpp"

World::World(sf::RenderWindow* _window)
{
	window = _window;
	configure(events); // configure base before all else
	// generate all systems
	systems.add<inputSystem>(events);
	systems.configure();

	// possibly load entity data from file
}

void World::update(sf::Time deltaTime)
{
	// send updates to all systems eg. systems.update<movementsystem>(deltaTime);
	systems.update<inputSystem>(deltaTime.asSeconds());
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
	window->close();
}
