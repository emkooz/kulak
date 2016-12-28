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
	// TODO: make customizable
	switch (keyboard.key)
	{
	case sf::Keyboard::Escape:
		kk::log("Quitting...");
		eventManager.emit<evQuit>();
		break;
	case sf::Keyboard::W:
		kk::log("Pressing W");
		eventManager.emit<evUp>(keyboard.pressed);
		break;
	case sf::Keyboard::A:
		kk::log("Pressing A");
		eventManager.emit<evLeft>(keyboard.pressed);
		break;
	case sf::Keyboard::S:
		kk::log("Pressing S");
		eventManager.emit<evDown>(keyboard.pressed);
		break;
	case sf::Keyboard::D:
		kk::log("Pressing D");
		eventManager.emit<evRight>(keyboard.pressed);
		break;
	}
}