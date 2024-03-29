#include "enemy_ai.hpp"

enemyAISystem::enemyAISystem(entityx::EntityManager& _entityManager, entityx::EventManager& _eventManager) :
	entityManager(_entityManager), eventManager(_eventManager)
{};

void enemyAISystem::configure(entityx::EventManager& eventManager)
{
	eventManager.subscribe<evHitEnemy>(*this);
}

void enemyAISystem::update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt)
{
	//std::vector<entityx::Entity> entitiesToDestroy;

	entities.each<cEnemyType, cPosition, cRenderable, cVelocity, cDirection, cAnimation>([dt, &entities, &events, this](entityx::Entity enemy, cEnemyType &type, cPosition &position, cRenderable &render, cVelocity &vel, cDirection &direction, cAnimation &anim)
	{
		// no other way to fill the componenthandle using only a single entity, I guess...
		// TODO: WARNING: this would break for multiplayer, create different solution
		sf::Vector2f playerPosition;
		entities.each<cPosition, cPlayerID>([&](entityx::Entity player, cPosition &position, cPlayerID &id)
		{
			playerPosition = player.component<cPosition>()->pos;
		});

		// get direction between enemy and player
		direction.angle = playerPosition - position.pos;
		// get hypotenuse
		float hyp = sqrt((direction.angle.x * direction.angle.x) + (direction.angle.y * direction.angle.y));
		// normalize it
		sf::Vector2f vector(direction.angle.x / hyp, direction.angle.y / hyp);

		if (vector.x < 0) // moving left
		{
			direction.right = false;
			anim.animations.setReversed(render.box, false); 
		}
		else
		{
			direction.right = true;
			anim.animations.setReversed(render.box, true);
		}

		position.pos.x += (vector.x * vel.velocity.x * dt);
		position.pos.y += (vector.y * vel.velocity.y * dt);
		render.box->setPosition(position.pos);

		// this manages the enemy's color state, indicator for getting hit.
		// maybe should move to its own function or be refactored in some other way
		if (render.box->getColor() == sf::Color::Red && type.hitDurationClock.getElapsedTime().asSeconds() > type.hitVisualDuration)
			render.box->setColor(sf::Color::White);

		// handle attacking. sends an "attack" per each player, tests if attack should be sent
		entities.each<cPlayerID, cPosition, cRenderable, cDirection>([enemy, this](entityx::Entity player, cPlayerID &_pID, cPosition &_position, cRenderable &_render, cDirection &_direction)
		{
			attack(enemy, player);
		});

		if (enemy.component<cHealth>()->hp <= 0)
		{
			events.emit<evEnemyDead>(enemy, cEnemyType(enemy.component<cEnemyType>()->type));
			enemy.destroy();
		}
	});
}

void enemyAISystem::attack(entityx::Entity enemy, entityx::Entity player)
{
	entityx::ComponentHandle<cPosition> pPos = player.component<cPosition>();
	entityx::ComponentHandle<cPosition> ePos = enemy.component<cPosition>();

	entityx::ComponentHandle<cDirection> pDir = player.component<cDirection>();
	entityx::ComponentHandle<cDirection> eDir = enemy.component<cDirection>();
	
	kk::weaponType wType;
	if (enemy.component<cWeaponEnemy>()->weapon.has_component<cRailWeapon>())
		wType = kk::WEAPON_RAIL;
	else if (enemy.component<cWeaponEnemy>()->weapon.has_component<cMeleeWeapon>())
		wType = kk::WEAPON_MELEE;
	else if (enemy.component<cWeaponEnemy>()->weapon.has_component<cProjectileWeapon>())
		wType = kk::WEAPON_PROJECTILE;
	else
		wType = kk::WEAPON_MELEE; // temporary. needs some sort of error out

	if (eDir->right) // facing to the right
	{
		if (wType == kk::WEAPON_MELEE)
		{
			// check if the player is within 50 pixels of the enemy, start attacking. will be made dynamic later
			if (std::abs(pPos->pos.x - ePos->pos.x) < 50)
			{
					eventManager.emit<evFireEnemy>(enemy, player, eDir.get(), wType);
			}
		}
		else if (wType == kk::WEAPON_MELEE)
		{
			// todo
		}
		else if (wType == kk::WEAPON_PROJECTILE)
		{
			auto hitbox = enemy.component<cWeaponEnemy>()->weapon.component<cWeaponHitbox>()->hitbox;

			if (hitbox.intersects(player.component<cRenderable>()->box->getGlobalBounds()))
			{
				eventManager.emit<evFireEnemy>(enemy, player, eDir.get(), wType);
			}
		}
	}
	else // facing to the left
	{
		if (wType == kk::WEAPON_MELEE)
		{
			if (std::abs(ePos->pos.x - pPos->pos.x) < 50)
			{
					eventManager.emit<evFireEnemy>(enemy, player, eDir.get(), wType);
			}
		}
		else if (wType == kk::WEAPON_MELEE)
		{
			// todo
		}
		else if (wType == kk::WEAPON_PROJECTILE)
		{
			auto hitbox = enemy.component<cWeaponEnemy>()->weapon.component<cWeaponHitbox>()->hitbox;

			if (hitbox.intersects(player.component<cRenderable>()->box->getGlobalBounds()))
			{
				eventManager.emit<evFireEnemy>(enemy, player, eDir.get(), wType);
			}
		}
	}
}

void enemyAISystem::takeDamage(entityx::Entity entity, int damage)
{
	// currently does not account for any resistances
	entity.component<cHealth>()->hp -= damage;
}

void enemyAISystem::receive(const evHitEnemy &event)
{
	// im not sure why i have to declare it first, errors doing it all on one line
	entityx::Entity enemy = event.enemy;
	enemy.component<cEnemyType>()->hitDurationClock.restart();
	enemy.component<cRenderable>()->box->setColor(sf::Color::Red);

	takeDamage(event.enemy, event.damage);
}