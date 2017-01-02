#include "input.hpp"

namespace kk
{
	static std::vector<bool> pressed(sf::Keyboard::KeyCount, false);

	void setPressed(const evKeyboard& keyboard)
	{
		pressed[keyboard.key] = keyboard.pressed;
		log("key: " + std::to_string(keyboard.key) + " set: " + (pressed[keyboard.key]? "true" : "false"));
	}

	bool getPressed(sf::Keyboard::Key key)
	{
		return pressed[key];
	}
}

inputSystem::inputSystem(entityx::EventManager& _eventManager) :
	eventManager(_eventManager)
{}

void inputSystem::configure(entityx::EventManager& eventManager)
{
	eventManager.subscribe<evKeyboard>(*this);
}

/*void inputSystem::update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt)
{} // are these args necessary?*/


void inputSystem::receive(const evKeyboard& keyboard)
{
	// TODO: make customizable
	kk::setPressed(keyboard);

	switch (keyboard.key)
	{
	case sf::Keyboard::Escape:
		kk::log("Quitting...");
		eventManager.emit<evQuit>();
		break;
	}

}