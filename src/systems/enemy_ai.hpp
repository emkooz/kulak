#pragma once
#include <entityx/entityx.h>
#include <SFML/Graphics.hpp>
#include <components/cAnimation.hpp>
#include <components/cPlayer.hpp>
#include <components/cEnemy.hpp>
#include "components/cRender.hpp"
#include <systems/weapons.hpp>
#include <components/events.hpp>

class enemyAISystem : public entityx::System<enemyAISystem>, public entityx::Receiver<enemyAISystem>
{
public:
	enemyAISystem(entityx::EntityManager& entityManager, entityx::EventManager& eventManager);
	void configure(entityx::EventManager& eventManager);
	void update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt);
	void receive(const evHitEnemy &event);

private:
	entityx::EntityManager& entityManager;
	entityx::EventManager& eventManager;
	void takeDamage(entityx::Entity entity, int damage);
	void attack(entityx::Entity enemy, entityx::Entity player);
};