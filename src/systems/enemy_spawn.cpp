#include "enemy_spawn.hpp"

enemySpawnSystem::enemySpawnSystem(entityx::EntityManager& _entityManager, entityx::EventManager& _eventManager, sf::RenderWindow* _window) :
	entityManager(_entityManager), eventManager(_eventManager), window(_window), rand(randomDevice()), spawnAvailable(false)
{};

void enemySpawnSystem::configure(entityx::EventManager& eventManager)
{
	eventManager.subscribe<evSpawnEnemy>(*this);
	eventManager.subscribe<evLevelCompleted>(*this);
	eventManager.subscribe<evLevelFailed>(*this);
	eventManager.subscribe<evEnemyDead>(*this);
	eventManager.subscribe<evBackgroundCreated>(*this);

	readSpawnFile();
}

void enemySpawnSystem::update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt)
{
	// handle spawning enemies based on a ruleset. currently just spawns enemies randomly
	/*if (kk::getPressed(sf::Keyboard::P))
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
	}*/ // set this to a debug mode?

	if (kk::getState() == kk::STATE_PLAYING)
	{
		if (enemiesToSpawn)
		{
			std::bernoulli_distribution randGen(0.5); // 50/50 spawning left or right
			auto randBool = std::bind(randGen, rand);
			std::uniform_int_distribution<int> offset((bounds->getBounds().top) + 60, -(bounds->getBounds().top) - 40);

			for (; enemiesToSpawn > 0; enemiesToSpawn--)
			{
				eventManager.emit<evSpawnEnemy>(levels[currentLevel - 1].types[enemiesToSpawn - 1],
					sf::Vector2f((randBool() ? bounds->getBounds().left - 50 : bounds->getBounds().left + bounds->getBounds().width + 50), // X coord of spawn
								 (offset(rand)))); // Y coord of spawn (+-offset from center)
			}
		}
	}

	if (enemiesToDelete.size() > 0)
	{
		for (int x = enemiesToDelete.size(); x > 0; x--)
		{
			enemiesToDelete[x - 1].destroy();
			enemiesToDelete.pop_back();
		}
	}
}

void enemySpawnSystem::spawnEnemy(int type, sf::Vector2f position)
{
	entityx::Entity enemy = entityManager.create();
	enemy.assign<cEnemyType>(type);
	enemy.assign<cSound>(); auto sound = enemy.component<cSound>();
	sound->spatial = true;

	if (type == 1)
	{
		enemy.assign<cHealth>(100);
		enemy.assign<cPosition>(position);
		enemy.assign<cDirection>(true);
		enemy.assign<cVelocity>(130.f, 130.f);
		std::shared_ptr<sf::Sprite> sprite(new sf::Sprite());
		sprite->setTexture(*kk::getTexture("ninja"));
		sprite->setTextureRect({ 0,0,0,0 });
		sprite->setPosition(position);
		sprite->setScale(0.2, 0.2);
		enemy.assign<cRenderable>(
			sprite,
			1,
			true);
		enemy.assign<cAnimation>(
			kk::getTexture("ninja"),
			5,
			20,
			sf::Vector2i(363, 458),
			10);
		enemy.component<cAnimation>()->animations.addAnimation("running", 11, 20);
		enemy.component<cAnimation>()->animations.setAnimation("running", false);

		//                                  ent,  weapon type,      name,   dmg,  cd
		eventManager.emit<evAddWeaponEnemy>(enemy, kk::WEAPON_MELEE, "knife", 5, 0.15f);

		std::shared_ptr<sf::Sound> weaponSound(new sf::Sound()); weaponSound->setBuffer(*kk::getSound("knife"));
		weaponSound->setPosition(position.x, position.y, 0);
		weaponSound->setAttenuation(0);
		sound->sounds.push_back(weaponSound);
		sound->names.emplace_back("knife");
	}
	else if (type == 2)
	{
		enemy.assign<cHealth>(100);
		enemy.assign<cPosition>(position);
		enemy.assign<cDirection>(true);
		enemy.assign<cVelocity>(130.f, 130.f);
		std::shared_ptr<sf::Sprite> sprite(new sf::Sprite());
		sprite->setTexture(*kk::getTexture("ninja"));
		sprite->setTextureRect({ 0,0,0,0 });
		sprite->setPosition(position);
		sprite->setScale(0.2, 0.2);
		enemy.assign<cRenderable>(
			sprite,
			1,
			true);
		enemy.assign<cAnimation>(
			kk::getTexture("ninja"),
			5,
			20,
			sf::Vector2i(363, 458),
			10);
		enemy.component<cAnimation>()->animations.addAnimation("running", 11, 20);
		enemy.component<cAnimation>()->animations.setAnimation("running", false);

		//                                  ent,  weapon type,      name,   dmg,  cd
		eventManager.emit<evAddWeaponEnemy>(enemy, kk::WEAPON_PROJECTILE, "proj", 10, 0.75f);

		std::shared_ptr<sf::Sound> weaponSound(new sf::Sound()); weaponSound->setBuffer(*kk::getSound("proj"));
		weaponSound->setPosition(position.x, 0, 0);
		weaponSound->setAttenuation(0);
		sound->sounds.push_back(weaponSound);
		sound->names.emplace_back("proj");
	}
}

void enemySpawnSystem::readSpawnFile()
{
	std::ifstream file("spawns.kk", std::ios::binary);
	std::vector<char> vBuffer((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
	std::string buffer(vBuffer.begin(), vBuffer.end());

	if (!file.fail() && buffer.size() > 0 && buffer.compare(0, 5, "KULAK") == 0) // make sure the header is correct
	{
		// read through file
		for (int pos = 5; pos < buffer.size(); pos++)
		{
			if (buffer[pos] == '>') // > == new level
				levels.emplace_back();
			else if (buffer[pos] == 0x01)
				levels.back().types.push_back(1);
			else if (buffer[pos] == 0x02)
				levels.back().types.push_back(2);
			else
				levels.back().types.push_back(1);
		}
		kk::log("levels size: " + std::to_string(levels.size()));
		enemiesAlive = levels[0].types.size();
		enemiesToSpawn = enemiesAlive;
		currentLevel = 1;
	}
	else
	{
		kk::log("Failed to load spawns.kk", kk::logType::error);
		// generate randomized spawns
	}
}

void enemySpawnSystem::receive(const evSpawnEnemy& enemy)
{
	spawnEnemy(enemy.type, enemy.pos);
}

void enemySpawnSystem::receive(const evLevelCompleted& event)
{
	if (levels.size() >= event.nextLevel)
	{
		enemiesAlive = levels[event.nextLevel - 1].types.size();
		enemiesToSpawn = enemiesAlive;
		currentLevel = event.nextLevel;
	}
	else // TODO: account for this
	{
		kk::log("No more levels left! Randomizing.");
		/* randomize here */
	}
}

void enemySpawnSystem::receive(const evLevelFailed& event)
{
	// delete every enemy, reset the spawn counter
	entityManager.each<cEnemyType>([this](entityx::Entity _entity, cEnemyType &_type)
	{
		enemiesToDelete.emplace_back(_entity);
	});

	enemiesAlive = levels[event.currentLevel - 1].types.size();
	enemiesToSpawn = enemiesAlive;
}

void enemySpawnSystem::receive(const evEnemyDead& event)
{
	enemiesAlive--;

	if (enemiesAlive == 0)
	{
		eventManager.emit<evAllEnemiesDead>();
	}
}

void enemySpawnSystem::receive(const evBackgroundCreated& event)
{
	bounds = event.bg;
}