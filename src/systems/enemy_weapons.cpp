#include "enemy_weapons.hpp"

enemyWeaponSystem::enemyWeaponSystem(entityx::EntityManager& _entityManager, entityx::EventManager& _eventManager) :
	entityManager(_entityManager), eventManager(_eventManager)
{};

void enemyWeaponSystem::configure(entityx::EventManager& eventManager)
{
	eventManager.subscribe<evEnemyDead>(*this);
	eventManager.subscribe<evAddWeaponEnemy>(*this);
	eventManager.subscribe<evFireEnemy>(*this);
	eventManager.subscribe<evBackgroundCreated>(*this);
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

	sf::FloatRect bounds = bg->getBounds();

	// update projectiles
	entityManager.each<cProjectile, cVelocity, cDirection, cPosition, cRenderable, cWeaponBase>([this, &dt, &bounds](entityx::Entity entity, cProjectile &proj, cVelocity &vel,
		cDirection &dir, cPosition &pos, cRenderable &render, cWeaponBase &base)
	{
		if (proj.owner == "enemy") // only checking projectiles that came from an enemy
		{
			pos.pos.x += ((dir.right ? 1 : -1) * (vel.velocity.x * dt));
			pos.pos.y += vel.velocity.y * dt;
			render.box->setPosition(pos.pos);

			// check collision between each player (use quadtree later)
			entityManager.each<cPlayerID, cRenderable>([this, &render, &base, &entity](entityx::Entity _entity, cPlayerID &_id, cRenderable &_render)
			{
				if (render.box->getGlobalBounds().intersects(_render.box->getGlobalBounds()))
				{
					eventManager.emit<evHitPlayer>(_entity, entity);
					entity.destroy();
				}
			});

			// kill projectile if it reaches the edges of the map + 50?
			if ((pos.pos.x > bounds.left + bounds.width + 50) || (pos.pos.x < bounds.left - 50))
				entity.destroy();
		}
	});
}

void enemyWeaponSystem::receive(const evFireEnemy &event)
{
	if (kk::getState() == kk::STATE_PLAYING)
	{
		entityx::Entity enemy = event.enemy;
		auto weapon = enemy.component<cWeaponEnemy>()->weapon;

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
			else if (event.type == kk::WEAPON_PROJECTILE)
			{
				auto pos = weapon.component<cPosition>()->pos;
				auto hitbox = weapon.component<cWeaponHitbox>();

				// spawn projectile
				entityx::Entity projectile = entityManager.create();
				projectile.assign<cPosition>(sf::Vector2f(
					(weapon.component<cDirection>()->right ? pos.x + hitbox->offset.x : pos.x - hitbox->offset.x),
					pos.y));
				projectile.assign<cVelocity>(weapon.component<cVelocity>()->velocity);
				projectile.assign<cDirection>(weapon.component<cDirection>()->right);
				projectile.assign<cProjectile>("enemy");

				std::shared_ptr<sf::Sprite> sprite(new sf::Sprite());
				sprite->setTexture(*kk::getTexture("coin"));
				sprite->setPosition(projectile.component<cPosition>()->pos);
				sprite->setScale(0.1f, 0.1f);
				projectile.assign<cRenderable>(
					sprite,
					3,
					true
					);

				projectile.assign<cProjectileOrigin>(false); // from an enemy
				projectile.assign<cWeaponBase>(weapon.component<cWeaponBase>());
			}

			eventManager.emit<evPlaySound>(enemy.component<cSound>(), weapon.component<cWeaponBase>()->name);
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

	if (event.weapon == kk::WEAPON_RAIL)
	{
		weapon.assign<cRailWeapon>();
	}
	else if (event.weapon == kk::WEAPON_MELEE)
	{
		weapon.assign<cWeaponBase>(event.name, event.cooldown, event.damage);

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
		sf::Vector2f velocity = sf::Vector2f(0.f, 0.f);
		weapon.assign<cProjectileWeapon>();
		if (event.name == "proj")
		{
			sf::FloatRect hitbox = sf::FloatRect(pPos.x + 32, pPos.y, 600.f, 10.f);
			weapon.assign<cWeaponHitbox>(hitbox, sf::Vector2f(2.f, 10.f));

			velocity = sf::Vector2f(600.f, 0.f);
		}

		weapon.assign<cVelocity>(velocity);
		weapon.assign<cWeaponBase>(event.name, event.cooldown, event.damage, velocity);
	}

	enemy.assign<cWeaponEnemy>(weapon);
}

void enemyWeaponSystem::receive(const evBackgroundCreated &event)
{
	bg = event.bg;
}
