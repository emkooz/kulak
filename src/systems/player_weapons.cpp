#include "player_weapons.hpp"

playerWeaponSystem::playerWeaponSystem(entityx::EntityManager& _entityManager, entityx::EventManager& _eventManager) :
	entityManager(_entityManager), eventManager(_eventManager), currentWeapon(0)
{};

void playerWeaponSystem::configure(entityx::EventManager& eventManager)
{
	eventManager.subscribe<evFire>(*this);
	eventManager.subscribe<evAddWeapon>(*this);
	eventManager.subscribe<entityx::ComponentAddedEvent<cPlayerID>>(*this);
}

void playerWeaponSystem::update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt)
{

	if (kk::getPressed(sf::Keyboard::Space))
	{
		if (currentWeapon == 0)
		{
			cRail rail(sf::Vector2f(weaponInventory[currentWeapon].component<cPosition>()->pos));
			cPlayerID pID(0);
			cPosition pos(sf::Vector2f(weaponInventory[currentWeapon].component<cPosition>()->pos));
			cDirection dir(pEntity.component<cDirection>()->right);
			// temporary. just to get something testable right now
			eventManager.emit<evFireRail>(rail, pID, pos, dir);
		}
	}

	entities.each<cPlayerID, cPosition>([this](entityx::Entity entity, cPlayerID& id, cPosition& pos)
	{
		for (int x = 0; x < weaponInventory.size(); x++)
		{
			weaponInventory[x].component<cPosition>()->pos.x = pos.pos.x;
			weaponInventory[x].component<cPosition>()->pos.y = pos.pos.y;
			weaponInventory[x].component<cRenderable>()->box->setPosition(pos.pos.x, pos.pos.y - 32);
		}
	});

}

void playerWeaponSystem::receive(const entityx::ComponentAddedEvent<cPlayerID> &event)
{
	pEntity = event.entity;
	currentWeapon = 0;
}

void playerWeaponSystem::receive(const evFire& event)
{
	if (currentWeapon == 0)
	{
		// temporary. just to get something testable right now
		/*cRail rail = cRail(sf::Vector2f(pEntity->component<cPosition>()->pos.x, pEntity->component<cPosition>()->pos.y));
		eventManager.emit<evFireRail>(rail, *pEntity->component<cPlayerID>().get(), *pEntity->component<cPosition>().get(), *pEntity->component<cDirection>().get());*/
	}
}

void playerWeaponSystem::receive(const evAddWeapon &event)
{
	// create a new weapon. right now it only makes a railgun
	weaponInventory.push_back({});
	currentWeapon = weaponInventory.size() - 1;
	weaponInventory[currentWeapon] = entityManager.create();
	weaponInventory[currentWeapon].assign<cPosition>(sf::Vector2f(0.f, 0.f));
	weaponInventory[currentWeapon].assign<cRail>(sf::Vector2f(0.f, 0.f));
	weaponInventory[currentWeapon].assign<cDirection>(true);
	std::unique_ptr<sf::Sprite> railSprite(new sf::Sprite());
	railSprite->setTexture(*kk::getTexture("ak"));
	railSprite->setScale(0.2, 0.2);
	weaponInventory[currentWeapon].assign<cRenderable>(
		std::move(railSprite),
		0,
		true);

}

// TODO: make fireWeapon() function
// TODO: make reversed based on last direction fired in, not moved in
// TODO: handle weapon entity being destroyed
