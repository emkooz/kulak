#include "enemy_spawn.hpp"

enemySpawnSystem::enemySpawnSystem(entityx::EntityManager& _entityManager, sf::RenderWindow* _window) :
	entityManager(_entityManager), window(_window), rand(randomDevice()), offset(-200, 200), spawnAvailable(false)
{};

void enemySpawnSystem::configure(entityx::EventManager& eventManager)
{
	eventManager.subscribe<evSpawnEnemy>(*this);
}

void enemySpawnSystem::update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt)
{
	// handle spawning enemies based on a ruleset. currently just spawns enemies randomly
	if (kk::getPressed(sf::Keyboard::P))
	{
		if (spawnAvailable)
		{
			spawnEnemy(0, { (float)sf::Mouse::getPosition(*window).x, (float)sf::Mouse::getPosition(*window).y });
			spawnAvailable = false;
		}
	}
	else
	{
		spawnAvailable = true;
	}

}

void enemySpawnSystem::spawnEnemy(int type, sf::Vector2f position)
{
	entityx::Entity enemy = entityManager.create();
	enemy.assign<cEnemyType>(0);
	enemy.assign<cHealth>(10);
	//sf::Vector2f position((800 / 2) + offset(rand), (600 / 2) + offset(rand));
	//kk::log("position: " + std::to_string(position.x) + "   " + std::to_string(position.y));
	enemy.assign<cPosition>(position);
	enemy.assign<cDirection>(true);
	enemy.assign<cVelocity>(130.f, 130.f);
	std::unique_ptr<sf::Sprite> sprite(new sf::Sprite());
	sprite->setTexture(*kk::getTexture("player"));
	sprite->setTextureRect({ 0,0,0,0 });
	sprite->setPosition(position);
	enemy.assign<cRenderable>(
		std::move(sprite),
		0,
		true);
	enemy.assign<cAnimation>(
		kk::getTexture("player"),
		8,
		65,
		sf::Vector2i(64, 64),
		10);
	enemy.component<cAnimation>()->animations.addAnimation("running", 5, 12);
	enemy.component<cAnimation>()->animations.setAnimation("running", false);
}

void enemySpawnSystem::receive(const evSpawnEnemy& enemy)
{
	spawnEnemy(enemy.type, enemy.pos);
}