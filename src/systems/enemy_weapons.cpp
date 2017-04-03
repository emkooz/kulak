#include "enemy_weapons.hpp"

enemyWeaponSystem::enemyWeaponSystem(entityx::EntityManager& _entityManager, entityx::EventManager& _eventManager) :
	entityManager(_entityManager), eventManager(_eventManager)
{};

void enemyWeaponSystem::configure(entityx::EventManager& eventManager)
{
	eventManager.subscribe<evEnemyDead>(*this);
	eventManager.subscribe<evAddWeaponEnemy>(*this);
	eventManager.subscribe<evFireEnemy>(*this);
}

void enemyWeaponSystem::update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt)
{
	entities.each<cEnemyType, cWeaponEnemy>([this](entityx::Entity enemy, cEnemyType &type, cWeaponEnemy &wEnemy)
	{
		auto hitbox = wEnemy.weapon.component<cWeaponHitbox>();

		if (enemy.component<cDirection>()->right) // moving to the right, point weapon to the right
		{
			wEnemy.weapon.component<cPosition>()->pos.x = enemy.component<cPosition>()->pos.x;
			wEnemy.weapon.component<cDirection>()->right = true;
			hitbox->hitbox.left = wEnemy.weapon.component<cPosition>()->pos.x + hitbox->offset.x; // offset + current position
		}
		else if (!enemy.component<cDirection>()->right) // moving to the left, point weapont to the left
		{
			wEnemy.weapon.component<cPosition>()->pos.x = enemy.component<cPosition>()->pos.x;
			wEnemy.weapon.component<cDirection>()->right = false;
			// |-------------| * * * * |------|
			// left      width offset    enemypos
			hitbox->hitbox.left = wEnemy.weapon.component<cPosition>()->pos.x - hitbox->offset.x - hitbox->hitbox.width;
		}

		wEnemy.weapon.component<cPosition>()->pos.y = enemy.component<cPosition>()->pos.y;
		hitbox->hitbox.top = wEnemy.weapon.component<cPosition>()->pos.y + hitbox->offset.y;
	});
}

void enemyWeaponSystem::receive(const evFireEnemy &event)
{
	entityx::Entity enemy = event.enemy, weapon = enemy.component<cWeaponEnemy>()->weapon;

	if (weapon.component<cWeaponBase>()->cooldownTimer.getElapsedTime().asSeconds() > weapon.component<cWeaponBase>()->cooldown) // off cooldown
	{
		weapon.component<cWeaponBase>()->cooldownTimer.restart();

		entityx::Entity player = event.player;
		sf::FloatRect gPlayerBounds = player.component<cRenderable>()->box->getGlobalBounds();

		if (event.type == kk::WEAPON_MELEE)
		{
			if (weapon.component<cWeaponHitbox>()->hitbox.intersects(gPlayerBounds))
			{
				eventManager.emit<evHitPlayer>(event.player, weapon);
			}
		}
	}
}

void enemyWeaponSystem::receive(const evEnemyDead &event)
{
	entityx::Entity enemy = event.ent;

	if (enemy.has_component<cWeaponEnemy>())
	{
		enemy.component<cWeaponEnemy>()->weapon.destroy();
		enemy.remove<cWeaponEnemy>();
	}
}

void enemyWeaponSystem::receive(const evAddWeaponEnemy &event)
{
	entityx::Entity weapon = entityManager.create();
	entityx::Entity enemy = event.ent;
	auto pPos = enemy.component<cPosition>()->pos;

	weapon.assign<cPosition>(pPos); // assign defaults, updated on next tick? or update it here since we already have the enemy its tied to
	weapon.assign<cDirection>(true);
	weapon.assign<cWeaponBase>(event.name, event.cooldown, event.damage);

	if (event.weapon == kk::WEAPON_RAIL)
	{
		weapon.assign<cRailWeapon>();
	}
	else if (event.weapon == kk::WEAPON_MELEE)
	{
		weapon.assign<cMeleeWeapon>();
		if (event.name == "knife")
		{
			// hitbox is 2 units away from center, 10 units above center, and is 50x10.
			sf::FloatRect hitbox = sf::FloatRect(pPos.x + 32, pPos.y + 10, 50.f, 10.f);
			weapon.assign<cWeaponHitbox>(hitbox, sf::Vector2f(2.f, 10.f));
		}
	}
	else if (event.weapon == kk::WEAPON_PROJECTILE)
	{
		weapon.assign<cProjectileWeapon>();
	}

	enemy.assign<cWeaponEnemy>(weapon);
}