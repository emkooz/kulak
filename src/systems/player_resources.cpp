#include "player_resources.hpp"

statsSystem::statsSystem(entityx::EntityManager& _entityManager, entityx::EventManager& _eventManager) :
	entityManager(_entityManager), eventManager(_eventManager)
{};

void statsSystem::configure(entityx::EventManager& eventManager)
{
	eventManager.subscribe<evPlayerCreated>(*this);
	eventManager.subscribe<evEnemyDead>(*this);
	eventManager.subscribe<evHitPlayer>(*this);
	eventManager.subscribe<evBuyHP>(*this);
	eventManager.subscribe<evBuyMana>(*this);
}

void statsSystem::update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt)
{
	if (pEntity.valid())
	{

	}
}

void statsSystem::receive(const evHitPlayer &event)
{
	entityx::Entity weapon = event.weapon;
	pStats.removeHealth(weapon.component<cWeaponBase>()->damage);
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
		pStats.addGold(10);
	}
}

void statsSystem::receive(const evBuyHP& event)
{
	int remainingGold = pStats.getGold() - 15;
	kk::log(std::to_string(remainingGold));
	if (remainingGold > 0)
	{
		pStats.setGold(remainingGold);
		pStats.addHealth(10);
	}
}

void statsSystem::receive(const evBuyMana& event)
{
	// TODO: lets not forget about this later, yeah?
	pStats.addGold(10);
}