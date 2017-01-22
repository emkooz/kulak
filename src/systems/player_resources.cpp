#include "player_resources.hpp"

statsSystem::statsSystem(entityx::EntityManager& _entityManager, entityx::EventManager& _eventManager) :
	entityManager(_entityManager), eventManager(_eventManager)
{};

void statsSystem::configure(entityx::EventManager& eventManager)
{
	eventManager.subscribe<evPlayerCreated>(*this);
	eventManager.subscribe<evEnemyDead>(*this);
}

void statsSystem::update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt)
{
	if (pEntity.valid())
	{

	}
}

void statsSystem::receive(const evPlayerCreated &event)
{
	pEntity = event.pEntity;
	pStats.configure(pEntity);
	eventManager.emit<evStatsCreated>(&pStats);
}

void statsSystem::receive(const evEnemyDead &event)
{
	if (event.type.type == 0)
	{
		pStats.changeGold(10);
	}
}
