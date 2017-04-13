#include "level_system.hpp"

levelSystem::levelSystem(entityx::EntityManager& _entityManager, entityx::EventManager& _eventManager, sf::RenderWindow* _window) :
	entityManager(_entityManager), eventManager(_eventManager), window(_window), currentLevel(1)
{};

void levelSystem::configure(entityx::EventManager& eventManager)
{
	eventManager.subscribe<evAllEnemiesDead>(*this);
}

void levelSystem::update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt)
{
}

void levelSystem::receive(const evAllEnemiesDead& enemy)
{
	currentLevel++;
	eventManager.emit<evLevelCompleted>(currentLevel);
}