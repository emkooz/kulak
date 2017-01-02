#pragma once
#include <vector>
#include <string>
#include <entityx/entityx.h>
#include <SFML/Window/Keyboard.hpp>
#include "log.hpp"
#include "state.hpp"

struct evKeyboard
{
	evKeyboard(sf::Keyboard::Key _key, bool _pressed) :
		key(_key),
		pressed(_pressed)
	{}

	sf::Keyboard::Key key;
	bool pressed;
};

class inputSystem : public entityx::System<inputSystem>, public entityx::Receiver<inputSystem>
{
public:
	inputSystem(entityx::EventManager& _eventManager);
	void configure(entityx::EventManager& eventManager) override;
	void update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt) override {};
	void receive(const evKeyboard& _evKeyboard);
	bool getPressed (sf::Keyboard::Key key);
private:
	void setPressed (const evKeyboard& keyboard);
	entityx::EventManager& eventManager;
	std::vector<bool> pressed;
};

namespace kk
{
	void setPressed (const evKeyboard& keyboard);
	bool getPressed (sf::Keyboard::Key key);
}

struct evLeft { evLeft(bool pressed) : pressed(pressed) {} bool pressed; };
struct evRight { evRight(bool pressed) : pressed(pressed) {} bool pressed; };
struct evUp { evUp(bool pressed) : pressed(pressed) {} bool pressed; };
struct evDown { evDown(bool pressed) : pressed(pressed) {} bool pressed; };