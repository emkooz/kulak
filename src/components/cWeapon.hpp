#pragma once
#include <SFML/Graphics.hpp>


struct cWeaponBase
{
	cWeaponBase(std::string name, float cooldown, int damage, float range, sf::Vector2f size) : name(name), damage(damage), cooldown(cooldown), pos({ 0.0f, 0.0f }), range(range), size(size) {}
	cWeaponBase(entityx::Entity enemy, std::string name, float cooldown, int damage, float range, sf::Vector2f size) : enemy(enemy), name(name), damage(damage), cooldown(cooldown), pos({ 0.0f, 0.0f }), range(range), size(size) {}
	entityx::Entity enemy; // not initialized if it is a weapon on the player
	std::string name;
	float cooldown; // delay per shot in seconds
	sf::Clock cooldownTimer;
	int damage;
	sf::Vector2f pos;
	float range;
	sf::Vector2f size;
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

namespace kk
{
	enum weaponType {WEAPON_RAIL, WEAPON_MELEE, WEAPON_PROJECTILE};
}