#include "player_movement.hpp"

movementSystem::movementSystem(entityx::EntityManager& _entityManager) : 
	entityManager(_entityManager)
{
}

void movementSystem::configure(entityx::EventManager& eventManager)
{
	/*eventManager.subscribe<evUp>(*this);
	eventManager.subscribe<evDown>(*this);
	eventManager.subscribe<evLeft>(*this);
	eventManager.subscribe<evRight>(*this);*/
}

void movementSystem::update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt)
{
	bool moveLeft = false, moveRight = false, moveDown = false, moveUp = false;

	if (kk::getPressed(sf::Keyboard::W))
		moveUp = true;
	if (kk::getPressed(sf::Keyboard::A))
		moveLeft = true;
	if (kk::getPressed(sf::Keyboard::S))
		moveDown = true;
	if (kk::getPressed(sf::Keyboard::D))
		moveRight = true;

	entities.each<cPlayerID, cPosition, cVelocity, cRenderable>([dt, moveUp, moveDown, moveLeft, moveRight](entityx::Entity entity, cPlayerID &player, cPosition &pos, cVelocity &vel, cRenderable &render)
	{
		// temporary solution, extremely basic
		if (moveUp)
		{
			if (moveDown)
				vel.y = 0.f;
			else
				vel.y = -320.f;
		}
		if (moveDown)
		{
			if (moveUp)
				vel.y = 0.f;
			else
				vel.y = 320.f;
		}
		if (moveLeft)
		{
			if (moveRight)
				vel.x = 0.f;
			else
				vel.x = -320.f;
		}
		if (moveRight)
		{
			if (moveLeft)
				vel.x = 0.f;
			else
				vel.x = 320.f;
		}
		if (!moveRight && !moveLeft)
			vel.x = 0.f;
		if (!moveUp && !moveDown)
			vel.y = 0.f;

		pos.pos.x += (vel.x * dt);
		pos.pos.y += (vel.y * dt);
		render.box->setPosition(pos.pos);
		// maybe use .move() instead of setPosition() and ditch position component?
	});
}

// maybe it's better to store a pointer to the player entity? instead of searching
// TODO SOON: check if vely is 320, if so make it 0, else make it -320, final else make it 0
// probably just better to do bool set on key down/key up and manage from there
/*void movementSystem::receive(const evUp& _evUp)
{
	entityManager.each<playerID, velocity>([_evUp](entityx::Entity entity, playerID &player, velocity &vel)
	{
		if (_evUp.pressed)
			vel.y = -320.f;
		else
			vel.y = 0.f;
	});
}

void movementSystem::receive(const evDown& _evDown)
{
	entityManager.each<playerID, velocity>([_evDown](entityx::Entity entity, playerID &player, velocity &vel)
	{
		if (_evDown.pressed)
			vel.y = 320.f;
		else
			vel.y = 0.f;
	});
}

void movementSystem::receive(const evLeft& _evLeft)
{
	entityManager.each<playerID, velocity>([_evLeft](entityx::Entity entity, playerID &player, velocity &vel)
	{
		if (_evLeft.pressed)
			vel.x = -320.f;
		else
			vel.x = 0.f;
	});
}

void movementSystem::receive(const evRight& _evRight)
{
	entityManager.each<playerID, velocity>([_evRight](entityx::Entity entity, playerID &player, velocity &vel)
	{
		if (_evRight.pressed)
			vel.x = 320.f;
		else
			vel.x = 0.f;
	});
}*/