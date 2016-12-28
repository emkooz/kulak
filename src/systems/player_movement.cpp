#include "player_movement.hpp"

movementSystem::movementSystem(entityx::EntityManager& _entityManager) : 
	entityManager(_entityManager)
{
}

void movementSystem::configure(entityx::EventManager& eventManager)
{
	eventManager.subscribe<evUp>(*this);
	eventManager.subscribe<evDown>(*this);
	eventManager.subscribe<evLeft>(*this);
	eventManager.subscribe<evRight>(*this);
}

void movementSystem::update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt)
{
	entities.each<playerID, position, velocity, renderable>([dt](entityx::Entity entity, playerID &player, position &pos, velocity &vel, renderable &render)
	{
		pos.pos.x += (vel.x * dt);
		pos.pos.y += (vel.y * dt);
		render.box->setPosition(pos.pos);
		// maybe use .move() instead of setPosition() and ditch position component?
	});
}

// maybe it's better to store a pointer to the player entity? instead of searching
// TODO SOON: check if vely is 320, if so make it 0, else make it -320, final else make it 0
// probably just better to do bool set on key down/key up and manage from there
void movementSystem::receive(const evUp& _evUp)
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
}