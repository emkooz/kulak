#pragma once
#include <entityx/entityx.h>
#include <SFML/Graphics.hpp>
#include "components/cPlayer.hpp"
#include "components/cWeapon.hpp"
#include "components/cAnimation.hpp"
#include "systems/weapons.hpp"
#include "input.hpp"
#include "log.hpp"
#include "resource.hpp"
#include <memory>
#include <vector>

// handles weapon states, current weapons, etc

struct evFire
{
	evFire(cDirection dir) : dir(dir) {}
	cDirection dir;
};

struct evAddWeapon
{
	evAddWeapon(kk::weaponType type, std::string name, std::string texture, int damage, float cooldown, float range, sf::Vector2f size) : weapon(type), name(name), texture(texture), damage(damage), cooldown(cooldown), range(range), size(size) {}
	kk::weaponType weapon;
	std::string name;
	std::string texture;
	int damage;
	float cooldown;
	float range;
	sf::Vector2f size;
};

class playerWeaponSystem : public entityx::System<playerWeaponSystem>, public entityx::Receiver<playerWeaponSystem>
{
public:
	playerWeaponSystem(entityx::EntityManager& entityManager, entityx::EventManager& _eventManager);
	void configure(entityx::EventManager& eventManager);
	void update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt);
	void receive(const evFire& event);
	void receive(const evAddWeapon& event);
	void receive(const entityx::ComponentAddedEvent<cPlayerID> &event);

private:
	entityx::EntityManager& entityManager;
	entityx::EventManager& eventManager;
	entityx::Entity pEntity;
	std::vector<entityx::Entity> weaponInventory;
	int currentWeapon;
	std::string currentDirection;
	//bool swappingWeapons;

	void pollWeaponSwap();
	void swapWeapons(int index);
};