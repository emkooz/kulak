#include "state.hpp"

namespace kk
{
	static gameState currentState = STATE_MENU;

	void setState(gameState state)
	{
		currentState = state;
	}

	gameState getState()
	{
		return currentState;
	}
}

stateSystem::stateSystem()
{
};

void stateSystem::configure(entityx::EventManager& eventManager)
{
	eventManager.subscribe<evSetState>(*this);
	kk::setState(kk::gameState::STATE_MENU);
	eventManager.emit<evSetState>(kk::STATE_MENU);
}

void stateSystem::update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt)
{
}

void stateSystem::receive(const evSetState &event)
{
	if (event.state != kk::getState())
		kk::setState(event.state);
}