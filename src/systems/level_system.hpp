#pragma once
#include <entityx/entityx.h>
#include "components/events.hpp"

class levelSystem : public entityx::System<levelSystem>, public entityx::Receiver<levelSystem>
{
public:
	levelSystem(entityx::EntityManager& entityManager, entityx::EventManager& eventManager, sf::RenderWindow* _window);
	void configure(entityx::EventManager& eventManager);
	void update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt);
	void receive(const evAllEnemiesDead& event);
private:
	entityx::EntityManager& entityManager;
	entityx::EventManager& eventManager;
	sf::RenderWindow* window;

	int currentLevel;
};