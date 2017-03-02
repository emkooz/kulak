#include "player_stats.hpp"

stats::stats() :
	movementSpeed(350)
{
}

// should happen basically instantly, player entity is created on game startup
void stats::configure(entityx::Entity pEntity)
{
	gold = pEntity.component<cGold>();
	health = pEntity.component<cHealth>();
	mana = pEntity.component<cMana>();
}

int stats::getGold()
{
	return gold->gold;
}

float stats::getMS()
{
	return movementSpeed;
}

int stats::getHealth()
{
	return health->hp;
}

int stats::getMana()
{
	return mana->mana;
}

void stats::setGold(int _gold)
{
	gold->gold = _gold;
}

void stats::addGold(int _gold)
{
	gold->gold += _gold;
}

void stats::removeGold(int _gold)
{
	gold->gold -= _gold;
}

void stats::changeSpeed(float _speed)
{
	movementSpeed += _speed;
}

void stats::setSpeed(float _speed)
{
	movementSpeed = _speed;
}

void stats::addHealth(int _hp)
{
	health->hp += _hp;
}

void stats::removeHealth(int _hp)
{
	health->hp -= _hp;
}

void stats::setHealth(int _hp)
{
	health->hp = _hp;
}

void stats::addMana(int _mana)
{
	mana->mana += _mana;
}

void stats::removeMana(int _mana)
{
	mana->mana -= _mana;
}

void stats::setMana(int _mana)
{
	mana->mana = _mana;
}