#include "input.hpp"

inputSystem::inputSystem(entityx::EventManager& _eventManager) :
	eventManager(_eventManager)
{}

void inputSystem::configure(entityx::EventManager& eventManager)
{
	eventManager.subscribe<evKeyboard>(*this);
}

void update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt)
{} // are these args necessary?

void inputSystem::receive(const evKeyboard& keyboard)
{
	if (keyboard.key == sf::Keyboard::Escape)
	{
		kk::log("Quitting...");
		eventManager.emit<evQuit>();
	}
}