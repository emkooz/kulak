#include "enemy_spawn.hpp"

enemySpawnSystem::enemySpawnSystem(entityx::EntityManager& _entityManager) :
	entityManager(_entityManager), rand(randomDevice()), offset(-200, 200)
{};

void enemySpawnSystem::configure(entityx::EventManager& eventManager)
{

}

void enemySpawnSystem::update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt)
{
	// handle spawning enemies based on a ruleset. currently just spawns enemies randomly
	if (((int)clock.getElapsedTime().asSeconds() % 10) == 0)
	{
		kk::log("creating enemy");
		entityx::Entity enemy = entities.create();
		enemy.assign<cHealth>(10);
		sf::Vector2f position((800/2) + offset(rand), (600 / 2) + offset(rand));
		kk::log("position: " + std::to_string(position.x) + "   " + std::to_string(position.y));
		enemy.assign<cPosition>(position);
		enemy.assign<cDirection>(true);
		enemy.assign<cVelocity>(0.f, 0.f);
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
}