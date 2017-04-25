#pragma once
#include <entityx/entityx.h>
#include "components/cPlayer.hpp"
#include "components/cRender.hpp"
#include "systems/input.hpp"
#include "systems/animation_system.hpp"
#include "background.hpp"
#include "player_stats.hpp"

class movementSystem : public entityx::System<movementSystem>, public entityx::Receiver<movementSystem>
{
public:
	movementSystem(entityx::EntityManager& _entityManager, entityx::EventManager& _eventManager);
	void configure(entityx::EventManager& eventManager);
	void update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt);
	void receive(const evBackgroundCreated &event);
	void receive(const evStatsCreated& event);
	void receive(const evLevelCompleted& event);
	void receive(const evLevelFailed& event);
	void receive(const evSetState& event);

private:
	entityx::EntityManager& entityManager;
	entityx::EventManager& eventManager;
	background* bg;
	stats* pStats;
};