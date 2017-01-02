#pragma once
#include <entityx/entityx.h>
#include "components/cPlayer.hpp"
#include "systems/input.hpp"

class movementSystem : public entityx::System<movementSystem>, public entityx::Receiver<movementSystem>
{
public:
	movementSystem(entityx::EntityManager& _entityManager);
	void configure(entityx::EventManager& eventManager);
	void update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt);
	/*void receive(const evUp& _evUp);
	void receive(const evDown& _evDown);
	void receive(const evLeft& _evLeft);
	void receive(const evRight& _evRight);*/

private:
	entityx::EntityManager& entityManager;
};