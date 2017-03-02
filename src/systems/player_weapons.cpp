#include "player_weapons.hpp"

playerWeaponSystem::playerWeaponSystem(entityx::EntityManager& _entityManager, entityx::EventManager& _eventManager) :
	entityManager(_entityManager), eventManager(_eventManager), currentWeapon(0), currentDirection("right")
{};

void playerWeaponSystem::configure(entityx::EventManager& eventManager)
{
	eventManager.subscribe<evFire>(*this);
	eventManager.subscribe<evAddWeapon>(*this);
	eventManager.subscribe<entityx::ComponentAddedEvent<cPlayerID>>(*this);
}

// TODO: this whole thing needs a rewrite
void playerWeaponSystem::update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt)
{
	// handle swapping weapons
	pollWeaponSwap();

	// handle firing the weapon
	if (kk::getPressed(sf::Keyboard::Left))
	{
		weaponInventory[currentWeapon].component<cDirection>()->right = false;
		sf::Sprite* box = weaponInventory[currentWeapon].component<cRenderable>()->box.get();
		weaponInventory[currentWeapon].component<cAnimation>()->animations.setReversed(box, true);
		currentDirection = "left";

		entityx::ComponentHandle<cWeaponBase> base = weaponInventory[currentWeapon].component<cWeaponBase>();

		if (base->cooldownTimer.getElapsedTime().asSeconds() > base->cooldown)
		{
			if (weaponInventory[currentWeapon].has_component<cRailWeapon>())
			{
				cPlayerID pID(0);
				// come from tip of weapon
				cPosition pos(sf::Vector2f(weaponInventory[currentWeapon].component<cPosition>()->pos + sf::Vector2f(((base->size.x * weaponInventory[currentWeapon].component<cRenderable>()->box->getScale().x) / 2), 0)));
				cDirection dir(weaponInventory[currentWeapon].component<cDirection>()->right);
				// temporary. just to get something testable right now
				eventManager.emit<evFireRail>(base, pID, pos, dir);
			}
			else if (weaponInventory[currentWeapon].has_component<cMeleeWeapon>())
			{
				cPlayerID pID(0);
				cPosition pos(sf::Vector2f(weaponInventory[currentWeapon].component<cPosition>()->pos));
				cDirection dir(weaponInventory[currentWeapon].component<cDirection>()->right);
				eventManager.emit<evFireMelee>(base, pID, pos, dir, weaponInventory[currentWeapon].component<cRenderable>());
			}

			base->cooldownTimer.restart();
		}
	}
	else if (kk::getPressed(sf::Keyboard::Right))
	{
		weaponInventory[currentWeapon].component<cDirection>()->right = true;
		sf::Sprite* box = weaponInventory[currentWeapon].component<cRenderable>()->box.get();
		weaponInventory[currentWeapon].component<cAnimation>()->animations.setReversed(box, false);
		currentDirection = "right";

		entityx::ComponentHandle<cWeaponBase> base = weaponInventory[currentWeapon].component<cWeaponBase>();

		if (base->cooldownTimer.getElapsedTime().asSeconds() > base->cooldown)
		{
			if (weaponInventory[currentWeapon].has_component<cRailWeapon>())
			{
				cPlayerID pID(0);
				cPosition pos(sf::Vector2f(weaponInventory[currentWeapon].component<cPosition>()->pos + sf::Vector2f(((base->size.x * weaponInventory[currentWeapon].component<cRenderable>()->box->getScale().x) / 2), 0)));
				cDirection dir(weaponInventory[currentWeapon].component<cDirection>()->right);
				// temporary. just to get something testable right now
				eventManager.emit<evFireRail>(base, pID, pos, dir);
			}
			else if (weaponInventory[currentWeapon].has_component<cMeleeWeapon>())
			{
				cPlayerID pID(0);
				cPosition pos(sf::Vector2f(weaponInventory[currentWeapon].component<cPosition>()->pos));
				cDirection dir(weaponInventory[currentWeapon].component<cDirection>()->right);
				eventManager.emit<evFireMelee>(base, pID, pos, dir, weaponInventory[currentWeapon].component<cRenderable>());
			}

			base->cooldownTimer.restart();
		}
	}

	// update the position of the weapon relative to the player. 
	entities.each<cPlayerID, cPosition>([this](entityx::Entity entity, cPlayerID& id, cPosition& pos)
	{
		for (int x = 0; x < weaponInventory.size(); x++)
		{
			if (currentDirection == "right")
				weaponInventory[x].component<cPosition>()->pos.x = pos.pos.x + 32;
			else if (currentDirection == "left")
				weaponInventory[x].component<cPosition>()->pos.x = pos.pos.x - 32;
			weaponInventory[x].component<cPosition>()->pos.y = pos.pos.y;
			weaponInventory[x].component<cRenderable>()->box->setPosition(weaponInventory[x].component<cPosition>()->pos.x, weaponInventory[x].component<cPosition>()->pos.y);
		}
	});

}

void playerWeaponSystem::pollWeaponSwap()
{
	// TODO: is there a better way of handling this?
	if (kk::getPressed(sf::Keyboard::Num1) && currentWeapon != 0)
		swapWeapons(0);
	else if (kk::getPressed(sf::Keyboard::Num2) && currentWeapon != 1)
		swapWeapons(1);
	else if (kk::getPressed(sf::Keyboard::Num3) && currentWeapon != 2)
		swapWeapons(2);
	else if (kk::getPressed(sf::Keyboard::Num4) && currentWeapon != 3)
		swapWeapons(3);
	else if (kk::getPressed(sf::Keyboard::Num5) && currentWeapon != 4)
		swapWeapons(4);
}

void playerWeaponSystem::swapWeapons(int index)
{
	if (index < weaponInventory.size())
	{
		currentWeapon = index;
		entityx::Entity cur = weaponInventory[currentWeapon];

		// if it is not facing the correct direction
		if ((currentDirection == "right" && !cur.component<cDirection>()->right) ||
			(currentDirection == "left" && cur.component<cDirection>()->right))
		{
			cur.component<cDirection>()->right = (currentDirection == "right" ? true : false);
			cur.component<cAnimation>()->animations.setReversed(cur.component<cRenderable>()->box.get(), !cur.component<cDirection>()->right);
		}

		for (int x = 0; x < weaponInventory.size(); x++)
		{
			if (x != currentWeapon)
				weaponInventory[x].component<cRenderable>()->render = false;
			else
				weaponInventory[x].component<cRenderable>()->render = true;
		}

		eventManager.emit<evSwitchWeapon>(index);
	}
}

void playerWeaponSystem::receive(const evAddWeapon &event)
{
	// Create a new weapon and put it in the inventory
	weaponInventory.push_back({});
	int index = weaponInventory.size() - 1;
	weaponInventory[index] = entityManager.create();
	weaponInventory[index].assign<cPosition>(sf::Vector2f(0.f, 0.f));
	weaponInventory[index].assign<cDirection>(true);
	weaponInventory[index].assign<cWeaponBase>(event.name, event.cooldown, event.damage, event.range, event.size);

	if (event.weapon == kk::WEAPON_RAIL)
		weaponInventory[index].assign<cRailWeapon>();
	else if (event.weapon == kk::WEAPON_MELEE)
		weaponInventory[index].assign<cMeleeWeapon>();
	else if (event.weapon == kk::WEAPON_PROJECTILE)
		weaponInventory[index].assign<cProjectileWeapon>();

	std::shared_ptr<sf::Sprite> weaponSprite(new sf::Sprite());
	weaponSprite->setTexture(*kk::getTexture(event.texture));
	weaponSprite->setScale(0.2, 0.2); // TODO: temporary
	weaponInventory[index].assign<cRenderable>(
		weaponSprite,
		3,
		index == 0? true : false);
	weaponInventory[index].assign<cAnimation>(
		kk::getTexture(event.texture),
		1,
		1,
		sf::Vector2i(256,256),
		10);
	weaponInventory[index].component<cAnimation>()->animations.addAnimation("idle", 1, 1);
	weaponInventory[index].component<cAnimation>()->animations.setAnimation("idle", false);

	//kk::log(std::to_string(&weaponInventory[index].id());
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
		/*cRailWeapon rail = cRailWeapon(sf::Vector2f(pEntity->component<cPosition>()->pos.x, pEntity->component<cPosition>()->pos.y));
		eventManager.emit<evFireRail>(rail, *pEntity->component<cPlayerID>().get(), *pEntity->component<cPosition>().get(), *pEntity->component<cDirection>().get());*/
	}
}

// TODO: make fireWeapon() function
// TODO: make reversed based on last direction fired in, not moved in
// TODO: handle weapon entity being destroyed
