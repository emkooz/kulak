#pragma once
#include <entityx/entityx.h>
#include <SFML/Graphics.hpp>
#include "components/events.hpp"
#include "components/cRender.hpp"
#include "resource.hpp"
#include "systems/input.hpp"

class menuSystem : public entityx::System<menuSystem>, public entityx::Receiver<menuSystem>
{
public:
	menuSystem(entityx::EntityManager& entityManager, entityx::EventManager& eventManager, sf::RenderWindow* window);
	void configure(entityx::EventManager& eventManager);
	void update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt);
	void receive(const evStatsCreated &event);
	void receive(const evSwitchWeapon &event);

private:
	entityx::EntityManager& entityManager;
	entityx::EventManager& eventManager;
	sf::RenderWindow* window;
	bool mousePressed;

	// HUD is constructed manually, since it never changes. update() handles updating strings and textures
	entityx::Entity playText;
	entityx::Entity optionsText;
	entityx::Entity quitText;
};