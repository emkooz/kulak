#pragma once
#include <vector>
#include <string>
#include <entityx/entityx.h>
#include <SFML/Window/Keyboard.hpp>
#include "log.hpp"
#include "state.hpp"
#include "components/events.hpp"

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