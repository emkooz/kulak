#include "world.hpp"

World::World(sf::RenderWindow* _window) :
	gui(entities, events)
{
	window = _window;
	configure(events); // configure base before all else
}

void World::update(sf::Time deltaTime)
{
	// send updates to all systems eg. systems.update<movementsystem>(deltaTime);
	systems.update<inputSystem>(deltaTime.asSeconds());
	if (kk::getState() == kk::gameState::STATE_PLAYING)
	{
		systems.update<movementSystem>(deltaTime.asSeconds());
		systems.update<animationSystem>(deltaTime.asSeconds());
		systems.update<enemySpawnSystem>(deltaTime.asSeconds());
		systems.update<enemyAISystem>(deltaTime.asSeconds());
		systems.update<weaponSystem>(deltaTime.asSeconds());
		systems.update<playerWeaponSystem>(deltaTime.asSeconds());
		systems.update<cameraSystem>(deltaTime.asSeconds());
		systems.update<statsSystem>(deltaTime.asSeconds());
		systems.update<hudSystem>(deltaTime.asSeconds());
		systems.update<enemyWeaponSystem>(deltaTime.asSeconds());
	}
	if (kk::getState() == kk::gameState::STATE_MENU)
	{
		systems.update<menuSystem>(deltaTime.asSeconds());
	}
	//systems.update<stateSystem>(deltaTime.asSeconds());

	// update GUI with the new values
	gui.update();
}

void World::createSystems()
{
	// generate all systems
	systems.add<inputSystem>(events);
	systems.add<movementSystem>(entities);
	systems.add<animationSystem>(entities);
	systems.add<enemySpawnSystem>(entities, events, window);
	systems.add<enemyAISystem>(entities, events);
	systems.add<weaponSystem>(entities, events);
	systems.add<playerWeaponSystem>(entities, events);
	systems.add<cameraSystem>(entities, window);
	systems.add<statsSystem>(entities, events);
	systems.add<hudSystem>(entities, events, window);
	systems.add<stateSystem>(events);
	systems.add<menuSystem>(entities, events, window);
	systems.add<enemyWeaponSystem>(entities, events);
	systems.add<levelSystem>(entities, events, window);
	systems.configure();
}

void World::createEntities(entityx::EventManager& event_manager)
{
	bg.load(entities, events);

	ePlayer = entities.create(); // does scope matter? could declare it in this function instead of class?
	ePlayer.assign<cHealth>(100);
	ePlayer.assign<cGold>(0);
	ePlayer.assign<cMana>(100);
	ePlayer.assign<cPlayerID>(0);
	ePlayer.assign<cPosition>(sf::Vector2f(0.f, 0.f));
	ePlayer.assign<cDirection>(true); // true = right, make enum for DIR_LEFT and DIR_RIGHT later
	ePlayer.assign<cVelocity>(0.f, 0.f);
	std::shared_ptr<sf::Sprite> pSprite(new sf::Sprite());
	pSprite->setTexture(*kk::getTexture("player"));
	pSprite->setTextureRect(sf::IntRect(0, 0, 0, 0));
	ePlayer.assign<cRenderable>(
		pSprite, // sf::Sprite
		2, // renderLayer
		true // render
		);
	ePlayer.assign<cAnimation>(
		kk::getTexture("player"), // sprite sheet
		8, // row size
		65, // total frames
		sf::Vector2i(64, 64), // each frame is 64x64px
		10); // runs at 10 frames per second
	ePlayer.component<cAnimation>()->animations.addAnimation("running", 5, 12);
	ePlayer.component<cAnimation>()->animations.addAnimation("idle", 65, 65);
	ePlayer.component<cAnimation>()->animations.setAnimation("idle", false);

	//                              weapon type,     name,     tex,   dmg, cd,    range,  size
	event_manager.emit<evAddWeapon>(kk::WEAPON_RAIL, "rail",   "ak",   20, 0.1f,  4000.f, sf::Vector2f(256, 256));
	event_manager.emit<evAddWeapon>(kk::WEAPON_MELEE, "knife", "knife", 5, 0.15f, 32.f,   sf::Vector2f(256, 256));

	event_manager.emit<evPlayerCreated>(ePlayer);

	// setup the GUI
	gui.init(window);
}

void World::configure(entityx::EventManager &event_manager)
{
	event_manager.subscribe<evPause>(*this);
	event_manager.subscribe<evResume>(*this);
	event_manager.subscribe<evQuit>(*this);
}

void World::pollGui(sf::Event ev)
{
	gui.pollEvents(ev);
}

void World::renderGui()
{
	gui.draw();
}

void World::receive(const evPause& pause)
{
	kk::log("Received a pause event.");
}

void World::receive(const evResume& resume)
{
	kk::log("Received a resume event.");
}

void World::receive(const evQuit& quit)
{
	// handle saving
	kk::log("Received a quit event.");
	kk::setState(kk::gameState::STATE_QUIT);
}
