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
#include "background.hpp"
#include "components/events.hpp"
#include <memory>
#include <vector>
#include <unordered_map>
#include <algorithm>

class enemyWeaponSystem : public entityx::System<enemyWeaponSystem>, public entityx::Receiver<enemyWeaponSystem>
{
public:
	enemyWeaponSystem(entityx::EntityManager& entityManager, entityx::EventManager& _eventManager);
	void configure(entityx::EventManager& eventManager);
	void update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt);
	void receive(const evAddWeaponEnemy& event);
	void receive(const evEnemyDead& event);
	void receive(const evFireEnemy& event);
	void receive(const evBackgroundCreated& event);

private:
	entityx::EntityManager& entityManager;
	entityx::EventManager& eventManager;
	background* bg;
};