#pragma once
#include <string>
#include <entityx/entityx.h>
#include "components/cAnimation.hpp"
#include "components/cPlayer.hpp"
#include "components/cRender.hpp"
#include "components/events.hpp"

class animationSystem : public entityx::System<animationSystem>, public entityx::Receiver<animationSystem>
{
public:
	animationSystem(entityx::EntityManager& entityManager);
	void configure(entityx::EventManager& eventManager);
	void update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt);
	void receive(const evPlayerAnimationSet& animation);
	void receive(const entityx::ComponentAddedEvent<cAnimation> &event);
	void animationSystem::receive(const evAddedLayerToAnimation &event);
private:
	entityx::EntityManager& entityManager;
};