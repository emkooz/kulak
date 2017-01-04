#pragma once
#include <random>
#include <memory>
#include <entityx/entityx.h>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "components/cAnimation.hpp"
#include "components/cPlayer.hpp"
#include "log.hpp"
#include "resource.hpp"

class enemySpawnSystem : public entityx::System<enemySpawnSystem>, public entityx::Receiver<enemySpawnSystem>
{
public:
	enemySpawnSystem(entityx::EntityManager& entityManager);
	void configure(entityx::EventManager& eventManager);
	void update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt);
private:
	entityx::EntityManager& entityManager;
	// random numbers
	std::random_device randomDevice;
	std::mt19937 rand;
	std::uniform_int_distribution<int> offset;
	// internal clock for timing spawning
	sf::Clock clock;
};