#pragma once
#include <entityx/entityx.h>
#include "player_stats.hpp"
#include "systems/enemy_ai.hpp"
#include "log.hpp"

// released after all components for player are added, ComponentAddedEvent only gets when pID is added
struct evPlayerCreated
{
	evPlayerCreated(entityx::Entity pEntity) : pEntity(pEntity) {}
	entityx::Entity pEntity;
};

// handles all player stats. gold, movement, hp, mana, etc
class statsSystem : public entityx::System<statsSystem>, public entityx::Receiver<statsSystem>
{
public:
	statsSystem(entityx::EntityManager& entityManager, entityx::EventManager& _eventManager);
	void configure(entityx::EventManager& eventManager);
	void update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt);
	void receive(const evPlayerCreated &event);
	void receive(const evEnemyDead &event);
	//void receive (const evTakeDamage &event);
	//void receive (const evGainHealth &event);
	// movementspeed change event

private:
	stats pStats;
	entityx::EntityManager& entityManager;
	entityx::EventManager& eventManager;
	entityx::Entity pEntity;
};