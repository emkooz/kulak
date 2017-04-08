#pragma once
#include <entityx/entityx.h>
#include <SFML/Graphics.hpp>
#include "components/cPlayer.hpp"
#include "components/cEnemy.hpp"
#include "components/cWeapon.hpp"
#include "components/cAnimation.hpp"
#include "components/cRender.hpp"

// forward declaration
class background;
class stats;
namespace kk
{
	enum gameState : unsigned int;
}

// handles weapon states, current weapons, etc
struct evFire
{
	evFire(cDirection dir) : dir(dir) {}
	cDirection dir;
};

struct evFireEnemy
{
	evFireEnemy(entityx::Entity enemy, entityx::Entity player, cDirection dir, kk::weaponType type) : 
		player(player), enemy(enemy), dir(dir), type(type) {}
	kk::weaponType type;
	entityx::Entity player;
	entityx::Entity enemy;
	cDirection dir;
};

struct evAddWeapon
{
	evAddWeapon(kk::weaponType type, std::string name, int damage, float cooldown) : weapon(type), name(name),
		damage(damage), cooldown(cooldown), velocity({ 0.f, 0.f }) {}
	evAddWeapon(kk::weaponType type, std::string name, int damage, float cooldown, sf::Vector2f velocity) : weapon(type), name(name),
		damage(damage), cooldown(cooldown), velocity(velocity) {}
	kk::weaponType weapon;
	std::string name;
	int damage;
	float cooldown;
	sf::Vector2f velocity;
};

struct evAddWeaponEnemy
{
	evAddWeaponEnemy(entityx::Entity ent, kk::weaponType type, std::string name, int damage, float cooldown) : ent(ent), weapon(type), name(name),
		damage(damage), cooldown(cooldown) {}
	entityx::Entity ent;
	kk::weaponType weapon;
	std::string name;
	int damage;
	float cooldown;
};

// when an enemy dies 
struct evEnemyDead
{
	evEnemyDead(entityx::Entity ent, cEnemyType type) : ent(ent), type(type) {}
	entityx::Entity ent;
	cEnemyType type;
};

// released when the background entity is created
struct evBackgroundCreated
{
	evBackgroundCreated(background* bg) : bg(bg) {}
	background* bg;
};

// animation related events
struct evPlayerAnimationSet
{
	evPlayerAnimationSet(int id, bool right, std::string name) : id(id), right(right), name(name) {}
	int id; // player id
	bool right; // facing the right direction or left (left == reversed)
	std::string name; // name of animation to change to
};

struct evEntityAnimationSet
{
	evEntityAnimationSet(entityx::Entity &ent) : ent(ent) {}
	entityx::Entity &ent;
};

struct evAddedLayerToAnimation
{
	evAddedLayerToAnimation(entityx::Entity &ent, int layer) : ent(ent), layer(layer) {}
	entityx::Entity &ent;
	int layer;
};

// event released when an enemy spawns
struct evSpawnEnemy
{
	evSpawnEnemy(int type = 0, sf::Vector2f pos = { 0.f, 0.f }) : type(type), pos(pos) {}
	int type;
	sf::Vector2f pos;
};


//input related events (mostly unused?)
struct evKeyboard
{
	evKeyboard(sf::Keyboard::Key _key, bool _pressed) :
		key(_key),
		pressed(_pressed)
	{}

	sf::Keyboard::Key key;
	bool pressed;
};

struct evLeft { evLeft(bool pressed) : pressed(pressed) {} bool pressed; };
struct evRight { evRight(bool pressed) : pressed(pressed) {} bool pressed; };
struct evUp { evUp(bool pressed) : pressed(pressed) {} bool pressed; };
struct evDown { evDown(bool pressed) : pressed(pressed) {} bool pressed; };

// released after all components for player are added, ComponentAddedEvent only gets when pID is added
struct evPlayerCreated
{
	evPlayerCreated(entityx::Entity pEntity) : pEntity(pEntity) {}
	entityx::Entity pEntity;
};

// State change events
struct evPause {};
struct evResume {};
struct evQuit {};

// weapon related events
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
	evFireMelee(entityx::ComponentHandle<cWeaponBase> melee, cPlayerID pID, cPosition pos, cDirection dir, entityx::ComponentHandle<cWeaponHitbox> hitbox) : melee(melee), pID(pID), pos(pos), dir(dir), hitbox(hitbox) {}
	entityx::ComponentHandle<cWeaponBase> melee;
	cPlayerID pID;
	cPosition pos;
	cDirection dir;
	entityx::ComponentHandle<cWeaponHitbox> hitbox;
};

struct evFireProjectile
{
	evFireProjectile(entityx::ComponentHandle<cWeaponBase> weapon, 
		int pID,
		entityx::ComponentHandle<cPosition> pos,
		entityx::ComponentHandle<cDirection> dir,
		entityx::ComponentHandle<cWeaponHitbox> hitbox) : weapon(weapon), pID(pID), pos(pos), dir(dir), hitbox(hitbox) {}

	entityx::ComponentHandle<cWeaponBase> weapon;
	int pID;
	entityx::ComponentHandle<cPosition> pos;
	entityx::ComponentHandle<cDirection> dir;
	entityx::ComponentHandle<cWeaponHitbox> hitbox;
};

struct evHitEnemy
{
	evHitEnemy(entityx::Entity enemy, float damage) : enemy(enemy), damage(damage) {}
	entityx::Entity enemy;
	float damage;
};

struct evHitPlayer
{
	evHitPlayer(entityx::Entity player, entityx::Entity weapon) : player(player), weapon(weapon) {}
	entityx::Entity player;
	entityx::Entity weapon;
};

struct evSwitchWeapon
{
	evSwitchWeapon(int weapon) : weapon(weapon) {}
	int weapon;
};

// handles player stats being created, anything that wants to modify the hud likely hooks into this
struct evStatsCreated
{
	evStatsCreated(stats* stat) : pStats(stat) {}
	stats* pStats;
};

// game state
struct evSetState
{
	evSetState(kk::gameState state) : state(state) {}
	kk::gameState state;
};

// background management
struct evChangeBackground
{
	evChangeBackground(std::string name) : name(name) {}
	std::string name;
};

struct evBuyHP
{
};

struct evBuyMana
{
};

struct evAllEnemiesDead
{
};

struct evLevelCompleted
{
	evLevelCompleted(int nextLevel) : nextLevel(nextLevel) {}
	int nextLevel;
};