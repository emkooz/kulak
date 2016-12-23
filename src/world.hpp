#pragma once
#include <entityx/entityx.h>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "systems/state.hpp"
#include "systems/input.hpp"
#include "log.hpp"

class World : public entityx::EntityX, public entityx::Receiver<World>
{
public:
	World(sf::RenderWindow* _window);
	void configure(entityx::EventManager &event_manager);
	void update(sf::Time deltaTime);
	void receive(const evPause& pause);
	void receive(const evResume& resume);
	void receive(const evQuit& quit);

private:
	sf::RenderWindow* window;
};