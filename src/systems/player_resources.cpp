#include "player_resources.hpp"

statsSystem::statsSystem(entityx::EntityManager& _entityManager, entityx::EventManager& _eventManager) :
	entityManager(_entityManager), eventManager(_eventManager)
{};

void statsSystem::configure(entityx::EventManager& eventManager)
{
	eventManager.subscribe<evPlayerCreated>(*this);
	eventManager.subscribe<evLevelCompleted>(*this);
	eventManager.subscribe<evLevelFailed>(*this);
	eventManager.subscribe<evEnemyDead>(*this);
	eventManager.subscribe<evHitPlayer>(*this);
	eventManager.subscribe<evBuyHP>(*this);
	eventManager.subscribe<evBuyMana>(*this);
	eventManager.subscribe<evBuyMS>(*this);
	eventManager.subscribe<evBuyGoldGain>(*this);
	eventManager.subscribe<evBuyMPS>(*this);
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

	if (pStats.getHealth() <= 0)
	{
		eventManager.emit<evSetState>(kk::STATE_PREGAME);
		eventManager.emit<evLevelFailed>(pStats.getCurrentLevel());
	}
}

void statsSystem::receive(const evPlayerCreated &event)
{
	pEntity = event.pEntity;
	pStats.configure(pEntity);
	eventManager.emit<evStatsCreated>(&pStats);
}

void statsSystem::receive(const evLevelCompleted& event)
{
	pStats.setCurrentLevel(event.nextLevel);
	pStats.setHealth(pStats.getMaxHP());
	pStats.setMana(pStats.getMaxMana());
	pStats.setGoldAtLevel(pStats.getGold());
}

void statsSystem::receive(const evLevelFailed& event)
{
	pStats.setHealth(pStats.getMaxHP());
	pStats.setMana(pStats.getMaxMana());
	pStats.setGold(pStats.getGoldAtLevel());
}

void statsSystem::receive(const evEnemyDead &event)
{
	int baseGold = 0;

	if (event.type.type == 0)
		baseGold = 10;

	pStats.addGold(baseGold + pStats.getGoldGain());
}

// Adds to >>MAX<< HP, not current
void statsSystem::receive(const evBuyHP& event)
{
	int remainingGold = pStats.getGold() - pStats.getUpgradeCost("maxHP");
	if (remainingGold >= 0)
	{
		pStats.setGold(remainingGold);
		pStats.addMaxHP(10);
		pStats.addUpgradeCost("maxHP", 5);
	}
}

// Same as mhp
void statsSystem::receive(const evBuyMana& event)
{
	int remainingGold = pStats.getGold() - pStats.getUpgradeCost("maxMana");
	if (remainingGold >= 0)
	{
		pStats.setGold(remainingGold);
		pStats.addMaxMana(10);
		pStats.addUpgradeCost("maxMana", 5);
	}
}

void statsSystem::receive(const evBuyMS& event)
{
	int remainingGold = pStats.getGold() - pStats.getUpgradeCost("MS");
	if (remainingGold >= 0)
	{
		pStats.setGold(remainingGold);
		pStats.changeSpeed(5);
		pStats.addUpgradeCost("MS", 15);
	}
}

void statsSystem::receive(const evBuyGoldGain& event)
{
	int remainingGold = pStats.getGold() - pStats.getUpgradeCost("goldGain");
	if (remainingGold >= 0)
	{
		pStats.setGold(remainingGold);
		pStats.addGoldGain(1);
		pStats.addUpgradeCost("goldGain", 35);
	}
}

void statsSystem::receive(const evBuyMPS& event)
{
	int remainingGold = pStats.getGold() - pStats.getUpgradeCost("manaPS");
	if (remainingGold >= 0)
	{
		pStats.setGold(remainingGold);
		pStats.addMPS(1);
		pStats.addUpgradeCost("manaPS", 40);
	}
}