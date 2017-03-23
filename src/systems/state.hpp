#pragma once
#include "components/events.hpp"

namespace kk
{
	enum gameState : unsigned int{ STATE_MENU, STATE_PLAYING, STATE_PREGAME, STATE_PAUSED, STATE_LOADING, STATE_QUIT };
	void setState(gameState state);
	gameState getState();
}

class stateSystem : public entityx::System<stateSystem>, public entityx::Receiver<stateSystem>
{
public:
	stateSystem(entityx::EventManager& _eventManager);
	entityx::EventManager& eventManager;
	void configure(entityx::EventManager& eventManager);
	void update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt);
	void receive(const evSetState &event);
	void receive(const evLevelCompleted& event);
};