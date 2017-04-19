#pragma once
#include <entityx/entityx.h>
#include <SFML/Graphics.hpp>
#include "components/cPlayer.hpp"
#include "components/cWeapon.hpp"
#include "components/cAnimation.hpp"
#include "components/cRender.hpp"
#include "systems/weapons.hpp"
#include "input.hpp"
#include "log.hpp"
#include "resource.hpp"
#include "player_stats.hpp"
#include "components/events.hpp"
#include <memory>
#include <vector>

class playerWeaponSystem : public entityx::System<playerWeaponSystem>, public entityx::Receiver<playerWeaponSystem>
{
public:
	playerWeaponSystem(entityx::EntityManager& entityManager, entityx::EventManager& _eventManager);
	void configure(entityx::EventManager& eventManager);
	void update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt);
	void receive(const evFire& event);
	void receive(const evAddWeapon& event);
	void receive(const evBuyDamage& event);
	void receive(const evBuyCooldown& event);
	void receive(const evStatsCreated& event);
	void receive(const entityx::ComponentAddedEvent<cPlayerID> &event);

private:
	entityx::EntityManager& entityManager;
	entityx::EventManager& eventManager;
	entityx::Entity pEntity;
	std::vector<entityx::Entity> weaponInventory;
	int currentWeapon;
	std::string currentDirection;
	//bool swappingWeapons;

	stats* pStats;

	void pollWeaponSwap();
	void swapWeapons(int index);
};