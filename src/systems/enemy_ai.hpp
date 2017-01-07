#pragma once
#include <entityx/entityx.h>
#include <SFML/Graphics.hpp>
#include <components/cAnimation.hpp>
#include <components/cPlayer.hpp>
#include <components/cEnemy.hpp>

class enemyAISystem : public entityx::System<enemyAISystem>, public entityx::Receiver<enemyAISystem>
{
public:
	enemyAISystem(entityx::EntityManager& entityManager);
	void configure(entityx::EventManager& eventManager);
	void update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt);

private:
	entityx::EntityManager& entityManager;
};