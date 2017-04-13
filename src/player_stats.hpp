#pragma once
#include <entityx/entityx.h>
#include "components/cPlayer.hpp"
#include "components/cRender.hpp"

class stats
{
public:
	stats();
	void configure(entityx::Entity pEntity);

	int getGold();
	float getMS();
	int getHealth();
	int getMana();
	unsigned int getMaxHP();
	unsigned int getMaxMana();
	unsigned int getGoldGain();
	float getManaPS();
	
	void setGold(int gold);
	void addGold(int gold);
	void removeGold(int gold);
	void addGoldGain(unsigned int goldGain);

	void changeSpeed(float speed);
	void setSpeed(float speed);

	void removeHealth(int hp);
	void addHealth(int hp);
	void setHealth(int hp);
	void addMaxHP(unsigned int maxHP);

	void addMana(int mana);
	void removeMana(int mana);
	void setMana(int mana);
	void addMaxMana(unsigned int maxMana);
	void addMPS(float mps);

	unsigned int getUpgradeCost(std::string attrib);
	void addUpgradeCost(std::string attrib, int val);

	void setCurrentLevel(int level);
	int getCurrentLevel();

	void setGoldAtLevel(int gold);
	int getGoldAtLevel();
private:
	entityx::ComponentHandle<cGold> gold;
	entityx::ComponentHandle<cHealth> health;
	entityx::ComponentHandle<cMana> mana;

	unsigned int maxHP, maxMana, goldGain;
	unsigned int maxHPUC, maxManaUC, goldGainUC, MSUC, manaPSUC; // UC = upgrade cost

	int currentLevel, goldAtLevel;

	float movementSpeed, manaPS; // PS = per second
};