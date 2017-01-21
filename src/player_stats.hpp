#pragma once
#include <entityx/entityx.h>
#include <components/cPlayer.hpp>

class stats
{
public:
	stats();
	void configure(entityx::Entity pEntity);

	int getGold();
	float getMS();
	int getHealth();
	int getMana();
	
	void changeGold(int gold);
	void changeSpeed(float speed);
	void setSpeed(float speed);
	void changeHealth(int hp);
	void setHealth(int hp);
	void changeMana(int mana);
	void setMana(int mana);
private:
	entityx::ComponentHandle<cGold> gold;
	entityx::ComponentHandle<cHealth> health;
	entityx::ComponentHandle<cMana> mana;
	float movementSpeed;
};