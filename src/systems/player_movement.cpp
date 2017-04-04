#include "player_movement.hpp"

movementSystem::movementSystem(entityx::EntityManager& _entityManager) : 
	entityManager(_entityManager)
{
}

void movementSystem::configure(entityx::EventManager& eventManager)
{
	eventManager.subscribe<evBackgroundCreated>(*this);
}

// TODO: major rewrite of movement system for player
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

	entities.each<cPlayerID, cPosition, cVelocity, cRenderable, cDirection, cCurrentWeapon>([dt, &events, moveUp, moveDown, moveLeft, moveRight, this](entityx::Entity entity, cPlayerID &player, cPosition &pos, cVelocity &vel, cRenderable &render, cDirection &direction, cCurrentWeapon &weapon)
	{
		// temporary solution, extremely basic
		if (moveUp)
		{
			if (moveDown)
				vel.y = 0.f;
			else
			{
				vel.y = -320.f;
				events.emit<evPlayerAnimationSet>(0, direction.right, weapon.name + "_running");
			}
		}
		if (moveDown)
		{
			if (moveUp)
				vel.y = 0.f;
			else
			{
				vel.y = 320.f;
				events.emit<evPlayerAnimationSet>(0, direction.right, weapon.name + "_running");
			}
		}
		if (moveLeft)
		{
			if (moveRight)
			{
				vel.x = 0.f;
			}
			else
			{
				vel.x = -320.f;
				events.emit<evPlayerAnimationSet>(0, false, weapon.name + "_running");
				direction.right = false;
			}
		}
		if (moveRight)
		{
			if (moveLeft)
			{
				vel.x = 0.f;
			}
			else
			{
				vel.x = 320.f;
				events.emit<evPlayerAnimationSet>(0, true, weapon.name + "_running");
				direction.right = true;
			}
		}
		if (!moveRight && !moveLeft)
			vel.x = 0.f;
		if (!moveUp && !moveDown)
			vel.y = 0.f;

		if (vel.x == 0 && vel.y == 0)
		{
			if (direction.right)
				events.emit<evPlayerAnimationSet>(0, true, weapon.name + "_idle");
			else
				events.emit<evPlayerAnimationSet>(0, false, weapon.name + "_idle");
		}
		
		float deltaPosX = vel.x * dt;
		float deltaPosY = vel.y * dt;

		// TODO: MAJOR: window size and player size is currently hardwired, fix
		if ((deltaPosY + pos.pos.y - 32) > (bg->getBounds().top + 50) &&
			(deltaPosY + pos.pos.y + 32) < 300)
			pos.pos.y += deltaPosY;
		else if ((deltaPosY + pos.pos.y - 32) < (bg->getBounds().top + 50) ) // hit top
			pos.pos.y = bg->getBounds().top + 50 + 32 + 1;
		else // hit bottom
			pos.pos.y = 300 - 32 - 1;

		sf::FloatRect bounds = bg->getBounds();
		if ((deltaPosX + pos.pos.x + 32) < (bounds.left + bounds.width) &&
			(deltaPosX + pos.pos.x - 32) > bounds.left)
			pos.pos.x += deltaPosX;
		else if ((deltaPosX + pos.pos.x + 32) > (bounds.left + bounds.width)) // hit right
			pos.pos.x = (bounds.left + bounds.width) - 32 - 1;
		else // hit left
			pos.pos.x = bounds.left + 32 + 1;

		render.box->setPosition(pos.pos);
		// maybe use .move() instead of setPosition() and ditch position component?
	});
}

void movementSystem::receive(const evBackgroundCreated &event)
{
	bg = event.bg;
}

// TODO: maybe it's better to store a pointer to the player entity? instead of searching
// TODO SOON: check if vely is 320, if so make it 0, else make it -320, final else make it 0
// probably just better to do bool set on key down/key up and manage from there
