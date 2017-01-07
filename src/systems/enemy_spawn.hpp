#pragma once
#include <random>
#include <memory>
#include <entityx/entityx.h>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "components/cAnimation.hpp"
#include "components/cPlayer.hpp"
#include "components/cEnemy.hpp"
#include "log.hpp"
#include "resource.hpp"
#include "systems/input.hpp"

struct evSpawnEnemy
{
	evSpawnEnemy(int type = 0, sf::Vector2f pos = { 0.f, 0.f }) : type(type), pos(pos) {}
	int type;
	sf::Vector2f pos;
};

class enemySpawnSystem : public entityx::System<enemySpawnSystem>, public entityx::Receiver<enemySpawnSystem>
{
public:
	enemySpawnSystem(entityx::EntityManager& entityManager, sf::RenderWindow* _window);
	void configure(entityx::EventManager& eventManager);
	void update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt);
	void receive(const evSpawnEnemy& enemy);
private:
	entityx::EntityManager& entityManager;
	sf::RenderWindow* window;
	// random numbers
	std::random_device randomDevice;
	std::mt19937 rand;
	std::uniform_int_distribution<int> offset;
	// internal clock for timing spawning
	sf::Clock clock;
	bool spawnAvailable;

	void spawnEnemy(int type, sf::Vector2f position);
};