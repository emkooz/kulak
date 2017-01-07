#include "enemy_ai.hpp"

enemyAISystem::enemyAISystem(entityx::EntityManager& _entityManager) :
	entityManager(_entityManager)
{};

void enemyAISystem::configure(entityx::EventManager& eventManager)
{
}

void enemyAISystem::update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt)
{
	entities.each<cEnemyType, cPosition, cRenderable, cVelocity, cDirection, cAnimation>([dt, &entities](entityx::Entity, cEnemyType &type, cPosition &position, cRenderable &render, cVelocity &vel, cDirection &direction, cAnimation &anim)
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
			anim.animations.setReversed(render, true); 
		}
		else
		{
			direction.right = true;
			anim.animations.setReversed(render, false);
		}

		position.pos.x += (vector.x * vel.x * dt);
		position.pos.y += (vector.y * vel.y * dt);
		render.box->setPosition(position.pos);
	});
}