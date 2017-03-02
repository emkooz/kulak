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
	// update the position of each weapon relative to the paired enemy
		for (int x = 0; x < weaponVector.size(); x++)
		{
			entityx::Entity enemy = weaponVector[x].component<cWeaponBase>()->enemy;
			entityx::ComponentHandle<cAnimation> anim = weaponVector[x].component<cAnimation>();

			if (enemy.component<cDirection>()->right) // moving to the right, point weapon to the right
			{
				weaponVector[x].component<cPosition>()->pos.x = enemy.component<cPosition>()->pos.x + 32;
				if (anim->animations.getReversed())
					anim->animations.setReversed(weaponVector[x].component<cRenderable>()->box.get(), false);
				weaponVector[x].component<cDirection>()->right = true;
			}
			else if (!enemy.component<cDirection>()->right) // moving to the left, point weapont to the left
			{
				weaponVector[x].component<cPosition>()->pos.x = enemy.component<cPosition>()->pos.x - 32;
				if (!anim->animations.getReversed())
					anim->animations.setReversed(weaponVector[x].component<cRenderable>()->box.get(), true);
				weaponVector[x].component<cDirection>()->right = false;
			}

			weaponVector[x].component<cPosition>()->pos.y = enemy.component<cPosition>()->pos.y;
			weaponVector[x].component<cRenderable>()->box->setPosition(weaponVector[x].component<cPosition>()->pos.x, weaponVector[x].component<cPosition>()->pos.y);
		}
}

void enemyWeaponSystem::receive(const evFireEnemy &event)
{
	entityx::Entity weapon = getWeapon(event.enemy);

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
	lastWeaponEnemy = weaponVector[weaponVector.size() - 1].component<cWeaponBase>()->enemy;
	int vectorIndex = weaponMap[event.ent]; // get index of weapon we want to delete
	weaponVector[weaponMap[event.ent]].destroy();
	std::swap(weaponVector[vectorIndex], weaponVector[weaponVector.size() - 1]); // swap the last added weapon and the one we want to delete
	weaponVector.pop_back(); // delete swapped weapon from vector
	weaponMap.erase(event.ent); // delete map entry contianing deleted weapon
	weaponMap[lastWeaponEnemy] = vectorIndex; // set map entry for last weapon to the vector position of where the weapon was deleted
}

void enemyWeaponSystem::receive(const evAddWeaponEnemy &event)
{
	weaponVector.push_back({});
	int index = weaponVector.size() - 1;
	weaponMap[event.ent] = index; // add new weapon to list of enemy weapons
	weaponVector[index] = entityManager.create();
	weaponVector[index].assign<cPosition>(sf::Vector2f(0.f, 0.f)); // assign defaults, updated on next tick? or update it here since we already have the enemy its tied to
	weaponVector[index].assign<cDirection>(true);
	weaponVector[index].assign<cWeaponBase>(event.ent, event.name, event.cooldown, event.damage, event.range, event.size);

	if (event.weapon == kk::WEAPON_RAIL)
		weaponVector[index].assign<cRailWeapon>();
	else if (event.weapon == kk::WEAPON_MELEE)
		weaponVector[index].assign<cMeleeWeapon>();
	else if (event.weapon == kk::WEAPON_PROJECTILE)
		weaponVector[index].assign<cProjectileWeapon>();

	std::shared_ptr<sf::Sprite> weaponSprite(new sf::Sprite());
	weaponSprite->setTexture(*kk::getTexture(event.texture));
	weaponSprite->setScale(0.2, 0.2); // TODO: temporary
	weaponVector[index].assign<cRenderable>(
		weaponSprite,
		3,
		true);
	weaponVector[index].assign<cAnimation>(
		kk::getTexture(event.texture),
		1,
		1,
		sf::Vector2i(256, 256),
		10);
	weaponVector[index].component<cAnimation>()->animations.addAnimation("idle", 1, 1);
	weaponVector[index].component<cAnimation>()->animations.setAnimation("idle", false);

	lastWeaponEnemy = event.ent;
}

// fetches weapon that is attached to passed in enemy entity
entityx::Entity enemyWeaponSystem::getWeapon(entityx::Entity enemy)
{
	return weaponVector[weaponMap[enemy]];
} 