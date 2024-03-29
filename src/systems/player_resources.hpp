#pragma once
#include <entityx/entityx.h>
#include "player_stats.hpp"
#include "systems/enemy_ai.hpp"
#include "log.hpp"
#include "state.hpp"
#include "components/events.hpp"

// handles all player stats. gold, movement, hp, mana, etc
class statsSystem : public entityx::System<statsSystem>, public entityx::Receiver<statsSystem>
{
public:
	statsSystem(entityx::EntityManager& entityManager, entityx::EventManager& _eventManager);
	void configure(entityx::EventManager& eventManager);
	void update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt);
	void receive(const evPlayerCreated &event);
	void receive(const evSetState& event);
	void receive(const evLevelCompleted& event);
	void receive(const evLevelFailed& event);
	void receive(const evEnemyDead &event);
	void receive (const evHitPlayer &event);
	void receive(const evBuyHP& event);
	void receive(const evBuyMana& event);
	void receive(const evBuyMS& event);
	void receive(const evBuyGoldGain& event);
	void receive(const evBuyMPS& event);
	//void receive (const evGainHealth &event);
	// movementspeed change event
	stats pStats;

private:
	entityx::EntityManager& entityManager;
	entityx::EventManager& eventManager;
	entityx::Entity pEntity;
};