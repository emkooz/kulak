#include "player_movement.hpp"

movementSystem::movementSystem(entityx::EntityManager& _entityManager, entityx::EventManager& _eventManager) : 
	entityManager(_entityManager), eventManager(_eventManager)
{
}

void movementSystem::configure(entityx::EventManager& eventManager)
{
	eventManager.subscribe<evBackgroundCreated>(*this);
	eventManager.subscribe<evStatsCreated>(*this);
	eventManager.subscribe<evLevelCompleted>(*this);
	eventManager.subscribe<evLevelFailed>(*this);
	eventManager.subscribe<evSetState>(*this);
}

// TODO: major rewrite of movement system for player
void movementSystem::update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt)
{
	if (kk::getState() == kk::STATE_PLAYING)
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

		entities.each<cPlayerID, cPosition, cVelocity, cRenderable, cDirection, cCurrentWeapon>([dt, &events, moveUp, moveDown, moveLeft, moveRight, this](entityx::Entity entity, cPlayerID &player, cPosition &pos, cVelocity &velocity, cRenderable &render, cDirection &direction, cCurrentWeapon &weapon)
		{
			auto vel = velocity.velocity;
			// temporary solution, extremely basic
			if (moveUp)
			{
				if (moveDown)
					vel.y = 0.f;
				else
				{
					vel.y = -pStats->getMS();
					events.emit<evPlayerAnimationSet>(0, direction.right, weapon.name + "_running");
				}
			}
			if (moveDown)
			{
				if (moveUp)
					vel.y = 0.f;
				else
				{
					vel.y = pStats->getMS();
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
					vel.x = -pStats->getMS();
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
					vel.x = pStats->getMS();
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

			auto halfSize = render.box->getGlobalBounds().width * render.box->getScale().x; // half of the width of the player

			// TODO: MAJOR: window size and player size is currently hardwired, fix
			if ((deltaPosY + pos.pos.y - halfSize) > (bg->getBounds().top + 50) &&
				(deltaPosY + pos.pos.y + halfSize) < 300)
				pos.pos.y += deltaPosY;
			else if ((deltaPosY + pos.pos.y - halfSize) < (bg->getBounds().top + 50)) // hit top
				pos.pos.y = bg->getBounds().top + 50 + halfSize + 1;
			else // hit bottom
				pos.pos.y = 300 - halfSize - 1;

			sf::FloatRect bounds = bg->getBounds();
			if ((deltaPosX + pos.pos.x + halfSize) < (bounds.left + bounds.width) &&
				(deltaPosX + pos.pos.x - halfSize) > bounds.left)
				pos.pos.x += deltaPosX;
			else if ((deltaPosX + pos.pos.x + halfSize) > (bounds.left + bounds.width)) // hit right
				pos.pos.x = (bounds.left + bounds.width) - halfSize - 1;
			else // hit left
				pos.pos.x = bounds.left + halfSize + 1;

			render.box->setPosition(pos.pos);
			// maybe use .move() instead of setPosition() and ditch position component?
		});
	}
}

void movementSystem::receive(const evBackgroundCreated &event)
{
	bg = event.bg;
}

void movementSystem::receive(const evStatsCreated &event)
{
	pStats = event.pStats;
}

void movementSystem::receive(const evLevelCompleted &event)
{
	entityManager.each<cPlayerID, cPosition, cVelocity, cRenderable, cCurrentWeapon>([this](entityx::Entity entity, cPlayerID &player, cPosition &pos, cVelocity &velocity, cRenderable &render, cCurrentWeapon &weapon)
	{
		pos.pos = sf::Vector2f(600.0f, 300.0f);
		velocity.velocity = sf::Vector2f(0.f, 0.f);
		render.box->setPosition(pos.pos);
		eventManager.emit<evPlayerAnimationSet>(0, false, weapon.name + "_idle");
	});
}

void movementSystem::receive(const evLevelFailed& event)
{
	entityManager.each<cPlayerID, cPosition, cVelocity, cRenderable, cCurrentWeapon>([this](entityx::Entity entity, cPlayerID &player, cPosition &pos, cVelocity &velocity, cRenderable &render, cCurrentWeapon &weapon)
	{
		pos.pos = sf::Vector2f(600.0f, 300.0f);
		velocity.velocity = sf::Vector2f(0.f, 0.f);
		render.box->setPosition(pos.pos);
		eventManager.emit<evPlayerAnimationSet>(0, false, weapon.name + "_idle");
	});
}

void movementSystem::receive(const evSetState& event)
{
	if (event.state == kk::STATE_PREGAME)
	{
		entityManager.each<cPlayerID, cPosition, cVelocity, cRenderable, cCurrentWeapon>([this](entityx::Entity entity, cPlayerID &player, cPosition &pos, cVelocity &velocity, cRenderable &render, cCurrentWeapon &weapon)
		{
			pos.pos = sf::Vector2f(600.0f, 300.0f);
			velocity.velocity = sf::Vector2f(0.f, 0.f);
			render.box->setPosition(pos.pos);
			eventManager.emit<evPlayerAnimationSet>(0, false, weapon.name + "_idle");
		});
	}
	else if (event.state == kk::STATE_PLAYING)
	{
		entityManager.each<cPlayerID, cPosition, cVelocity, cRenderable, cCurrentWeapon>([this](entityx::Entity entity, cPlayerID &player, cPosition &pos, cVelocity &velocity, cRenderable &render, cCurrentWeapon &weapon)
		{
			pos.pos = sf::Vector2f(0.f, 0.f);
			velocity.velocity = sf::Vector2f(0.f, 0.f);
			render.box->setPosition(pos.pos);
			eventManager.emit<evPlayerAnimationSet>(0, false, weapon.name + "_idle");
		});
	}
}

// TODO: maybe it's better to store a pointer to the player entity? instead of searching
// TODO SOON: check if vely is 320, if so make it 0, else make it -320, final else make it 0
// probably just better to do bool set on key down/key up and manage from there
