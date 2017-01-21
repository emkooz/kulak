#include "enemy_ai.hpp"

enemyAISystem::enemyAISystem(entityx::EntityManager& _entityManager) :
	entityManager(_entityManager)
{};

void enemyAISystem::configure(entityx::EventManager& eventManager)
{
	eventManager.subscribe<evHitEnemy>(*this);
}

void enemyAISystem::update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt)
{
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
			anim.animations.setReversed(render.box.get(), true); 
		}
		else
		{
			direction.right = true;
			anim.animations.setReversed(render.box.get(), false);
		}

		position.pos.x += (vector.x * vel.x * dt);
		position.pos.y += (vector.y * vel.y * dt);
		render.box->setPosition(position.pos);

		// this manages the enemy's color state, indicator for getting hit.
		// maybe should move to its own function or be refactored in some other way
		if (render.box->getColor() == sf::Color::Red && type.hitDurationClock.getElapsedTime().asSeconds() > type.hitVisualDuration)
			render.box->setColor(sf::Color::White);

		if (enemy.component<cHealth>()->hp <= 0)
		{
			events.emit<evEnemyDead>(cEnemyType(enemy.component<cEnemyType>()->type));
			enemy.destroy();
		}
	});
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