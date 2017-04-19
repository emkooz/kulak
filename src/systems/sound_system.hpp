#pragma once
#include <SFML/Audio.hpp>
#include <entityx/entityx.h>
#include "components/cSound.hpp"
#include "components/events.hpp"
#include "log.hpp"

class soundSystem : public entityx::System<soundSystem>, public entityx::Receiver<soundSystem>
{
public:
	soundSystem(entityx::EntityManager& entityManager, entityx::EventManager& eventManager);
	void configure(entityx::EventManager& eventManager);
	void update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt);
	void receive(const entityx::ComponentAddedEvent<cPlayerID> &event);
	void receive(const evPlaySound &event);

private:
	entityx::EntityManager& entityManager;
	entityx::EventManager& eventManager;
	entityx::Entity pEntity;
};