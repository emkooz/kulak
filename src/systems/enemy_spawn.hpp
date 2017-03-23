#pragma once
#include <random>
#include <memory>
#include <vector>
#include <fstream>
#include <string>
#include <entityx/entityx.h>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "components/cAnimation.hpp"
#include "components/cPlayer.hpp"
#include "components/cRender.hpp"
#include "components/cEnemy.hpp"
#include "log.hpp"
#include "resource.hpp"
#include "background.hpp"
#include "systems/input.hpp"
#include "components/events.hpp"

class enemySpawnSystem : public entityx::System<enemySpawnSystem>, public entityx::Receiver<enemySpawnSystem>
{
public:
	enemySpawnSystem(entityx::EntityManager& entityManager, entityx::EventManager& eventManager, sf::RenderWindow* _window);
	void configure(entityx::EventManager& eventManager);
	void update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt);
	void receive(const evSpawnEnemy& enemy);
	void receive(const evLevelCompleted& event);
	void receive(const evEnemyDead& event);
	void receive(const evBackgroundCreated& event);

private:
	entityx::EntityManager& entityManager;
	entityx::EventManager& eventManager;
	sf::RenderWindow* window;

	// random numbers
	std::random_device randomDevice;
	std::mt19937 rand;
	std::uniform_int_distribution<int> offset;

	// internal clock for timing spawning
	sf::Clock clock;

	background* bounds;

	bool spawnAvailable;
	void spawnEnemy(int type, sf::Vector2f position);

	struct level
	{
		// order of vector is order that enemies should be spawned in
		std::vector<int> types;
	};
	std::vector<level> levels;
	int currentLevel;
	int enemiesAlive;
	int enemiesToSpawn;

	void readSpawnFile();
};