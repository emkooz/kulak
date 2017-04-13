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

stateSystem::stateSystem(entityx::EntityManager& _entityManager, entityx::EventManager& _eventManager) : entityManager(_entityManager), eventManager(_eventManager)
{
};

void stateSystem::configure(entityx::EventManager& eventManager)
{
	eventManager.subscribe<evSetState>(*this);
	eventManager.subscribe<evLevelCompleted>(*this);
	eventManager.subscribe<evLevelFailed>(*this);
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

	if (event.state == kk::STATE_MENU)
	{
		eventManager.emit<evChangeBackground>("menu-bg");
	}
	else if (event.state == kk::STATE_PREGAME)
	{
		eventManager.emit<evChangeBackground>("statbg");
		entityManager.each<cPlayerID, cRenderable, cAnimationLayered, cCurrentWeapon>([this](entityx::Entity entity, cPlayerID &player, cRenderable &render, cAnimationLayered& anim, cCurrentWeapon& weapon)
		{
			// TODO: fix scale changing when multiple layers are added
			render.box->setPosition(sf::Vector2f(600.0f, 300.0f));
			render.box->setScale(0.75, 0.75);
			eventManager.emit<evPlayerAnimationSet>(0, true, weapon.name + "_idle");
		});
	}
	else if (event.state == kk::STATE_PLAYING)
	{
		eventManager.emit<evChangeBackground>("bg");
		entityManager.each<cPlayerID, cRenderable, cAnimationLayered, cCurrentWeapon>([this](entityx::Entity entity, cPlayerID &player, cRenderable &render, cAnimationLayered& anim, cCurrentWeapon& weapon)
		{
			render.box->setPosition(sf::Vector2f(0.f, 0.f));
			render.box->setScale(0.2, 0.2);
			eventManager.emit<evPlayerAnimationSet>(0, true, weapon.name + "_idle");
		});
	}
}

void stateSystem::receive(const evLevelCompleted& event)
{
	eventManager.emit<evSetState>(kk::STATE_PREGAME);
}

void stateSystem::receive(const evLevelFailed& event)
{
	eventManager.emit<evSetState>(kk::STATE_PREGAME);
}