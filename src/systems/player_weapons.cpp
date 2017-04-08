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
		currentDirection = "left";

		entityx::ComponentHandle<cWeaponBase> base = weaponInventory[currentWeapon].component<cWeaponBase>();

		if (base->cooldownTimer.getElapsedTime().asSeconds() > base->cooldown)
		{
			entityx::ComponentHandle<cWeaponHitbox> hitbox = weaponInventory[currentWeapon].component<cWeaponHitbox>();
			auto weapon = weaponInventory[currentWeapon];

			if (weapon.has_component<cRailWeapon>())
			{
				cPlayerID pID(0);
				cPosition pos(weapon.component<cPosition>()->pos);
				cDirection dir(weapon.component<cDirection>()->right);
				// temporary. just to get something testable right now
				eventManager.emit<evFireRail>(base, pID, pos, dir);
			}
			else if (weapon.has_component<cMeleeWeapon>())
			{
				cPlayerID pID(0);
				cPosition pos(sf::Vector2f(weapon.component<cPosition>()->pos));
				cDirection dir(weapon.component<cDirection>()->right);
				eventManager.emit<evFireMelee>(base, pID, pos, dir, hitbox);
			}
			else if (weapon.has_component<cProjectileWeapon>())
			{
				eventManager.emit<evFireProjectile>(base, 0, weapon.component<cPosition>(), weapon.component<cDirection>(), weapon.component<cWeaponHitbox>());
			}

			base->cooldownTimer.restart();
		}
	}
	else if (kk::getPressed(sf::Keyboard::Right))
	{
		weaponInventory[currentWeapon].component<cDirection>()->right = true;
		currentDirection = "right";

		entityx::ComponentHandle<cWeaponBase> base = weaponInventory[currentWeapon].component<cWeaponBase>();
		auto weapon = weaponInventory[currentWeapon];

		if (base->cooldownTimer.getElapsedTime().asSeconds() > base->cooldown)
		{
			entityx::ComponentHandle<cWeaponHitbox> hitbox = weapon.component<cWeaponHitbox>();

			if (weapon.has_component<cRailWeapon>())
			{
				cPlayerID pID(0);
				cPosition pos(weapon.component<cPosition>()->pos); 
				cDirection dir(weapon.component<cDirection>()->right);
				// temporary. just to get something testable right now
				eventManager.emit<evFireRail>(base, pID, pos, dir);
			}
			else if (weapon.has_component<cMeleeWeapon>())
			{
				cPlayerID pID(0);
				cPosition pos(sf::Vector2f(weapon.component<cPosition>()->pos));
				cDirection dir(weapon.component<cDirection>()->right);
				eventManager.emit<evFireMelee>(base, pID, pos, dir, hitbox);
			}
			else if (weapon.has_component<cProjectileWeapon>())
			{
				eventManager.emit<evFireProjectile>(base, 0, weapon.component<cPosition>(), weapon.component<cDirection>(), weapon.component<cWeaponHitbox>());
			}

			base->cooldownTimer.restart();
		}
	}

	entities.each<cPlayerID, cPosition, cAnimationLayered>([this](entityx::Entity entity, cPlayerID& id, cPosition& pos, cAnimationLayered& anim)
	{
		for (int x = 0; x < weaponInventory.size(); x++)
		{
			weaponInventory[x].component<cPosition>()->pos = pos.pos;
			auto hitbox = weaponInventory[x].component<cWeaponHitbox>();
			hitbox->hitbox.left =
				currentDirection == "right" ? // facing right
				pos.pos.x + hitbox->offset.x : // set to right sided hitbox
				pos.pos.x - hitbox->offset.x - hitbox->hitbox.width; // else set to left sided hitbox
			hitbox->hitbox.top = pos.pos.y + hitbox->offset.y;
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
			//cur.component<cAnimation>()->animations.setReversed(cur.component<cRenderable>()->box.get(), !cur.component<cDirection>()->right);
		}

		/*for (int x = 0; x < weaponInventory.size(); x++)
		{
			if (x != currentWeapon)
				weaponInventory[x].component<cRenderable>()->render = false;
			else
				weaponInventory[x].component<cRenderable>()->render = true;
		}*/

		pEntity.component<cCurrentWeapon>()->name = cur.component<cWeaponBase>()->name;
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
	weaponInventory[index].assign<cWeaponBase>(event.name, event.cooldown, event.damage, event.velocity);

	auto pPos = pEntity.component<cPosition>()->pos;

	if (event.weapon == kk::WEAPON_RAIL)
	{
		weaponInventory[index].assign<cRailWeapon>();
		if (event.name == "rail")
		{
			sf::FloatRect hitbox = sf::FloatRect(pPos.x + 32, pPos.y, 1000.f, 2.f);
			weaponInventory[index].assign<cWeaponHitbox>(hitbox, sf::Vector2f(2.f, 0.f));
		}
	}
	else if (event.weapon == kk::WEAPON_MELEE)
	{
		weaponInventory[index].assign<cMeleeWeapon>();
		if (event.name == "knife")
		{
			sf::FloatRect hitbox = sf::FloatRect(pPos.x + 32, pPos.y + 10, 50.f, 10.f);
			weaponInventory[index].assign<cWeaponHitbox>(hitbox, sf::Vector2f(2.f, 10.f));
		}
	}
	else if (event.weapon == kk::WEAPON_PROJECTILE)
	{
		weaponInventory[index].assign<cProjectileWeapon>();
		if (event.name == "proj")
		{
			sf::FloatRect hitbox = sf::FloatRect(pPos.x + 32, pPos.y + 10, 50.f, 10.f);
			weaponInventory[index].assign<cWeaponHitbox>(hitbox, sf::Vector2f(2.f, 10.f));
		}
	}
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
