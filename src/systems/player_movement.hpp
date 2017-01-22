#pragma once
#include <entityx/entityx.h>
#include "components/cPlayer.hpp"
#include "components/cRender.hpp"
#include "systems/input.hpp"
#include "systems/animation_system.hpp"
#include "background.hpp"

class movementSystem : public entityx::System<movementSystem>, public entityx::Receiver<movementSystem>
{
public:
	movementSystem(entityx::EntityManager& _entityManager);
	void configure(entityx::EventManager& eventManager);
	void update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt);
	void receive(const evBackgroundCreated &event);

private:
	entityx::EntityManager& entityManager;
	background* bg;
};