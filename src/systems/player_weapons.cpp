#include "player_weapons.hpp"

playerWeaponSystem::playerWeaponSystem(entityx::EntityManager& _entityManager, entityx::EventManager& _eventManager) :
	entityManager(_entityManager), eventManager(_eventManager), currentWeapon("none")
{};

void playerWeaponSystem::configure(entityx::EventManager& eventManager)
{
	eventManager.subscribe<evFire>(*this);
	eventManager.subscribe<entityx::ComponentAddedEvent<cPlayerID>>(*this);
}

void playerWeaponSystem::update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt)
{

	if (kk::getPressed(sf::Keyboard::Space))
	{
		kk::log("lalala");
		if (currentWeapon == "rail")
		{
			entities.each<cPlayerID, cPosition, cDirection, cRail>([this](entityx::Entity entity, cPlayerID& pID, cPosition& pos, cDirection& dir, cRail& rail)
			{
				kk::log("pre firing");
				// temporary. just to get something testable right now
				kk::log("firing");
				eventManager.emit<evFireRail>(rail, pID, pos, dir);
			});
		}
	}

}

void playerWeaponSystem::receive(const entityx::ComponentAddedEvent<cPlayerID> &event)
{
	kk::log("now");
	pEntity = const_cast<entityx::Entity*>(&event.entity);
	currentWeapon = "rail";
}

void playerWeaponSystem::receive(const evFire& event)
{
	if (currentWeapon == "rail")
	{
		// temporary. just to get something testable right now
		cRail rail = cRail(sf::Vector2f(pEntity->component<cPosition>()->pos.x, pEntity->component<cPosition>()->pos.y));
		eventManager.emit<evFireRail>(rail, *pEntity->component<cPlayerID>().get(), *pEntity->component<cPosition>().get(), *pEntity->component<cDirection>().get());
	}
}

// TODO: make fireWeapon() function
// TODO: make reversed based on last direction fired in, not moved in
