#pragma once
#include <SFML/Graphics.hpp>


struct cWeaponBase
{
	cWeaponBase(std::string name, float cooldown, int damage) : name(name), damage(damage), cooldown(cooldown) {}
	std::string name;
	float cooldown; // delay per shot in seconds
	sf::Clock cooldownTimer;
	int damage;
};

struct cCurrentWeapon
{
	cCurrentWeapon(std::string name) : name(name) {}
	std::string name;
};

struct cWeaponEnemy
{
	cWeaponEnemy(entityx::Entity weapon) : weapon(weapon) {}
	entityx::Entity weapon;
};

struct cRailWeapon
{
};

struct cMeleeWeapon
{
};

struct cProjectileWeapon
{
};

struct cWeaponHitbox
{
	cWeaponHitbox(sf::FloatRect hitbox, sf::Vector2f offset) : hitbox(hitbox), offset(offset) {}
	cWeaponHitbox(float left, float top, float width, float height, sf::Vector2f offset) : hitbox(sf::FloatRect(left, top, width, height)), offset(offset) {}
	sf::FloatRect hitbox;
	sf::Vector2f offset; // this is how far away the hitbox is from the center of sprite. used in calculating new hitbox on direction reversal
	
};

namespace kk
{
	enum weaponType {WEAPON_RAIL, WEAPON_MELEE, WEAPON_PROJECTILE};
}