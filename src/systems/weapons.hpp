#pragma once
#include <entityx/entityx.h>
#include <SFML/Graphics.hpp>
#include <components/cWeapon.hpp>
#include <components/cPlayer.hpp>
#include <components/cEnemy.hpp>
#include <components/cAnimation.hpp>
#include "log.hpp"
#include <vector>

// handles gun management (ammo, state, etc), position?, all properties like damage, and hitscan collisions

struct evFireRail
{
	evFireRail(entityx::ComponentHandle<cWeaponBase> rail, cPlayerID pID, cPosition pos, cDirection dir) : rail(rail), pID(pID), pos(pos), dir(dir) {}
	entityx::ComponentHandle<cWeaponBase> rail;
	cPlayerID pID;
	cPosition pos;
	cDirection dir;
};

struct evFireMelee
{
	evFireMelee(entityx::ComponentHandle<cWeaponBase> melee, cPlayerID pID, cPosition pos, cDirection dir, entityx::ComponentHandle<cRenderable> sprite) : melee(melee), pID(pID), pos(pos), dir(dir), sprite(sprite) {}
	entityx::ComponentHandle<cWeaponBase> melee;
	cPlayerID pID;
	cPosition pos;
	cDirection dir;
	entityx::ComponentHandle<cRenderable> sprite;
};

struct evFireProjectile
{

};

struct evHitEnemy
{
	evHitEnemy(entityx::Entity enemy, float damage) : enemy(enemy), damage(damage) {}
	entityx::Entity enemy;
	float damage;
};

class weaponSystem : public entityx::System<weaponSystem>, public entityx::Receiver<weaponSystem>
{
public:
	weaponSystem(entityx::EntityManager& entityManager, entityx::EventManager& eventManager);
	void configure(entityx::EventManager& eventManager);
	void update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt);
	void receive(const evFireRail& rail);
	void receive(const evFireMelee& melee);

private:
	entityx::EntityManager& entityManager;
	entityx::EventManager& eventManager;
	bool same_sign(float a, float b);
	int line_intersects(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3, sf::Vector2f p4, sf::Vector2f& collision);
	std::vector<entityx::Entity> rails;
};