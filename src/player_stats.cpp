#include "player_stats.hpp"

stats::stats() :
	movementSpeed(400), goldGain(0), manaPS(0), maxHP(100), maxMana(100),
	maxHPUC(15), maxManaUC(15), goldGainUC(20), MSUC(5), manaPSUC(20),
	currentLevel(1), goldAtLevel(0)
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

unsigned int stats::getMaxHP()
{
	return maxHP;
}

unsigned int stats::getMaxMana()
{
	return maxMana;
}

unsigned int stats::getGoldGain()
{
	return goldGain;
}

float stats::getManaPS()
{
	return manaPS;
}

void stats::addGoldGain(unsigned int gg)
{
	goldGain += gg;
}

void stats::addMaxHP(unsigned int mhp)
{
	maxHP += mhp;
}

void stats::addMaxMana(unsigned int mm)
{
	maxMana += mm;
}

void stats::addMPS(float mps)
{
	manaPS += mps;
}

unsigned int stats::getUpgradeCost(std::string attrib)
{
	if (attrib == "maxHP")
		return maxHPUC;
	else if (attrib == "maxMana")
		return maxManaUC;
	else if (attrib == "goldGain")
		return goldGainUC;
	else if (attrib == "MS")
		return MSUC;
	else if (attrib == "manaPS")
		return manaPSUC;
}

void stats::addUpgradeCost(std::string attrib, int val)
{
	if (attrib == "maxHP")
		maxHPUC += val;
	else if (attrib == "maxMana")
		maxManaUC += val;
	else if (attrib == "goldGain")
		goldGainUC += val;
	else if (attrib == "MS")
		MSUC += val;
	else if (attrib == "manaPS")
		manaPSUC += val;
}

void stats::setCurrentLevel(int level)
{
	currentLevel = level;
}

int stats::getCurrentLevel()
{
	return currentLevel;
}

void stats::setGoldAtLevel(int gold)
{
	goldAtLevel = gold;
}

int stats::getGoldAtLevel()
{
	return goldAtLevel;
}