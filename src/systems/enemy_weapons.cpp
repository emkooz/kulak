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
		entityx::ComponentHandle<cAnimation> anim = wEnemy.weapon.component<cAnimation>();

		if (enemy.component<cDirection>()->right) // moving to the right, point weapon to the right
		{
			wEnemy.weapon.component<cPosition>()->pos.x = enemy.component<cPosition>()->pos.x + 32;
			if (anim->animations.getReversed())
				anim->animations.setReversed(wEnemy.weapon.component<cRenderable>()->box.get(), false);
			wEnemy.weapon.component<cDirection>()->right = true;
		}
		else if (!enemy.component<cDirection>()->right) // moving to the left, point weapont to the left
		{
			wEnemy.weapon.component<cPosition>()->pos.x = enemy.component<cPosition>()->pos.x - 32;
			if (!anim->animations.getReversed())
				anim->animations.setReversed(wEnemy.weapon.component<cRenderable>()->box.get(), true);
			wEnemy.weapon.component<cDirection>()->right = false;
		}

		wEnemy.weapon.component<cPosition>()->pos.y = enemy.component<cPosition>()->pos.y;
		wEnemy.weapon.component<cRenderable>()->box->setPosition(wEnemy.weapon.component<cPosition>()->pos.x, wEnemy.weapon.component<cPosition>()->pos.y);
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
		sf::FloatRect localBounds = weapon.component<cRenderable>()->box->getLocalBounds();
		sf::FloatRect globalBounds = weapon.component<cRenderable>()->box->getGlobalBounds();
		sf::Vector2f scale = weapon.component<cRenderable>()->box->getScale();
		bool rDir = event.dir.right; // right direction (vs. left == false)

		if (event.type == kk::WEAPON_MELEE)
		{
			sf::RectangleShape hitbox;
			hitbox.setSize(sf::Vector2f(localBounds.width * scale.x, localBounds.height * scale.y));
			hitbox.setOrigin(sf::Vector2f(hitbox.getSize().x / 2, hitbox.getSize().y / 2));
			hitbox.setPosition(sf::Vector2f(globalBounds.left + ((localBounds.width * fabs(scale.x)) / 2), globalBounds.top + ((localBounds.height * fabs(scale.y)) / 2)));

			if (hitbox.getGlobalBounds().intersects(gPlayerBounds))
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

	weapon.assign<cPosition>(sf::Vector2f(0.f, 0.f)); // assign defaults, updated on next tick? or update it here since we already have the enemy its tied to
	weapon.assign<cDirection>(true);
	weapon.assign<cWeaponBase>(event.name, event.cooldown, event.damage, event.range, event.size);

	if (event.weapon == kk::WEAPON_RAIL)
		weapon.assign<cRailWeapon>();
	else if (event.weapon == kk::WEAPON_MELEE)
		weapon.assign<cMeleeWeapon>();
	else if (event.weapon == kk::WEAPON_PROJECTILE)
		weapon.assign<cProjectileWeapon>();

	std::shared_ptr<sf::Sprite> weaponSprite(new sf::Sprite());
	weaponSprite->setTexture(*kk::getTexture(event.texture));
	weaponSprite->setScale(0.2, 0.2); // TODO: temporary
	weapon.assign<cRenderable>(
		weaponSprite,
		3,
		true);
	weapon.assign<cAnimation>(
		kk::getTexture(event.texture),
		1,
		1,
		sf::Vector2i(256, 256),
		10);

	weapon.component<cAnimation>()->animations.addAnimation("idle", 1, 1);
	weapon.component<cAnimation>()->animations.setAnimation("idle", false);

	enemy.assign<cWeaponEnemy>(weapon);
}