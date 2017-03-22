#include "enemy_spawn.hpp"

enemySpawnSystem::enemySpawnSystem(entityx::EntityManager& _entityManager, entityx::EventManager& _eventManager, sf::RenderWindow* _window) :
	entityManager(_entityManager), eventManager(_eventManager), window(_window), rand(randomDevice()), offset(-200, 200), spawnAvailable(false)
{};

void enemySpawnSystem::configure(entityx::EventManager& eventManager)
{
	eventManager.subscribe<evSpawnEnemy>(*this);

	readSpawnFile();
}

void enemySpawnSystem::update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt)
{
	// handle spawning enemies based on a ruleset. currently just spawns enemies randomly
	if (kk::getPressed(sf::Keyboard::P))
	{
		if (spawnAvailable)
		{
			spawnEnemy(0, window->mapPixelToCoords({ sf::Mouse::getPosition(*window).x, sf::Mouse::getPosition(*window).y }));
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
	//kk::log(std::to_string(enemy));
	enemy.assign<cEnemyType>(0);
	enemy.assign<cHealth>(100);
	//sf::Vector2f position((800 / 2) + offset(rand), (600 / 2) + offset(rand));
	//kk::log("position: " + std::to_string(position.x) + "   " + std::to_string(position.y));
	enemy.assign<cPosition>(position);
	enemy.assign<cDirection>(true);
	enemy.assign<cVelocity>(130.f, 130.f);
	std::shared_ptr<sf::Sprite> sprite(new sf::Sprite());
	sprite->setTexture(*kk::getTexture("player"));
	sprite->setTextureRect({ 0,0,0,0 });
	sprite->setPosition(position);
	enemy.assign<cRenderable>(
		sprite,
		1,
		true);
	enemy.assign<cAnimation>(
		kk::getTexture("player"),
		8,
		65,
		sf::Vector2i(64, 64),
		10);
	enemy.component<cAnimation>()->animations.addAnimation("running", 5, 12);
	enemy.component<cAnimation>()->animations.setAnimation("running", false);

	//                                   weapon type,              name,     tex, dmg, cd,    range,  size
	eventManager.emit<evAddWeaponEnemy>(enemy, kk::WEAPON_MELEE, "knife", "knife", 5, 0.15f, 32.f, sf::Vector2f(256, 256));
}

void enemySpawnSystem::readSpawnFile()
{

}

void enemySpawnSystem::receive(const evSpawnEnemy& enemy)
{
	spawnEnemy(enemy.type, enemy.pos);
}