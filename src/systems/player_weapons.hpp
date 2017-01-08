#pragma once
#include <entityx/entityx.h>
#include <SFML/Graphics.hpp>
#include "components/cPlayer.hpp"
#include "components/cWeapon.hpp"
#include "systems/weapons.hpp"
#include "input.hpp"
#include "log.hpp"
#include <memory>

// handles weapon states, current weapons, etc

struct evFire
{
	evFire(cDirection dir) : dir(dir) {}
	cDirection dir;
};

class playerWeaponSystem : public entityx::System<playerWeaponSystem>, public entityx::Receiver<playerWeaponSystem>
{
public:
	playerWeaponSystem(entityx::EntityManager& entityManager, entityx::EventManager& _eventManager);
	void configure(entityx::EventManager& eventManager);
	void update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt);
	void receive(const evFire& event);
	void receive(const entityx::ComponentAddedEvent<cPlayerID> &event);

private:
	entityx::EntityManager& entityManager;
	entityx::EventManager& eventManager;
	entityx::Entity* pEntity;
	std::string currentWeapon;
};