#pragma once
#include <entityx/entityx.h>
#include <SFML/Graphics.hpp>
#include <components/cWeapon.hpp>
#include <components/cPlayer.hpp>
#include "components/cRender.hpp"
#include <components/cEnemy.hpp>
#include <components/cAnimation.hpp>
#include "components/events.hpp"
#include "log.hpp"
#include <vector>

// handles gun management (ammo, state, etc), position?, all properties like damage, and hitscan collisions

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