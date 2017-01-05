#include "enemy_ai.hpp"

enemyAISystem::enemyAISystem(entityx::EntityManager& _entityManager) :
	entityManager(_entityManager)
{};

void enemyAISystem::configure(entityx::EventManager& eventManager)
{
}

void enemyAISystem::update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt)
{

}