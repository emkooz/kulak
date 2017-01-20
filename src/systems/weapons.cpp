#include "weapons.hpp"

weaponSystem::weaponSystem(entityx::EntityManager& _entityManager, entityx::EventManager& _eventManager) :
	entityManager(_entityManager), eventManager(_eventManager)
{};

void weaponSystem::configure(entityx::EventManager& eventManager)
{
	eventManager.subscribe<evFireRail>(*this);
	eventManager.subscribe<evFireMelee>(*this);
}

void weaponSystem::update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt)
{
	/*entities.each<cRail, cPlayerID, cRenderable>([](entityx::Entity entity, cRail &rail, cPlayerID &id, cRenderable &render)
	{
		rail.pos = { render.box->getPosition().x + render.box->getLocalBounds().width, (render.box->getPosition().y + render.box->getLocalBounds().height) / 2 };
	});*/
}

void weaponSystem::receive(const evFireRail& _rail)
{
	// loop through each enemy checking for a collision
	// later on use quadtrees
	// TODO: set tracer to an actual texture/animation
	entityManager.each<cEnemyType, cPosition, cRenderable>([this, _rail](entityx::Entity entity, cEnemyType &type, cPosition &pos, cRenderable &render)
	{
		float railX = _rail.pos.pos.x, railY = _rail.pos.pos.y;
		sf::FloatRect enemyGlobal = render.box->getGlobalBounds(), enemyLocal = render.box->getLocalBounds();
		// if the player is on the left side of the enemy and we are shooting right (this is kinda assuming its only shooting straight forward)
		if (railX <= pos.pos.x && _rail.dir.right)
		{
			sf::Vector2f result;
			// TODO: replace "4000" with the actual distance
			if (line_intersects({ railX, railY }, { railX + 4000, railY }, { enemyGlobal.left, enemyGlobal.top }, { enemyGlobal.left + enemyLocal.width, enemyGlobal.top }, result) || // top of box
				line_intersects({ railX, railY }, { railX + 4000, railY }, { enemyGlobal.left, enemyGlobal.top }, { enemyGlobal.left, enemyGlobal.top + enemyLocal.height }, result) || // left of box
				line_intersects({ railX, railY }, { railX + 4000, railY }, { enemyGlobal.left, enemyGlobal.top + enemyLocal.height }, { enemyGlobal.left + enemyLocal.width, enemyGlobal.top + enemyLocal.height }, result)) // bottom of box
			{
				// create rail to draw
				rails.push_back({});
				entityx::Entity railHit = entityManager.create();
				std::unique_ptr<sf::RectangleShape> rect(new sf::RectangleShape());
				rect->setSize({ result.x - railX, 5.f });
				rect->setPosition(_rail.pos.pos);
				rect->setFillColor(sf::Color::White);
				railHit.assign<cBasicRail>(
					std::move(rect),
					3,
					true);

				eventManager.emit<evHitEnemy>(entity, 20);
			}
		}
		// if the player is on the right side of the enemy and shooting left (same warning applies)
		else if (railX > pos.pos.x && !_rail.dir.right)
		{
			sf::Vector2f result;
			if (line_intersects({ railX, railY }, { railX - 4000, railY }, { enemyGlobal.left, enemyGlobal.top }, { enemyGlobal.left + enemyLocal.width, enemyGlobal.top }, result) || // top of box
				line_intersects({ railX, railY }, { railX - 4000, railY }, { enemyGlobal.left + enemyLocal.width, enemyGlobal.top }, { enemyGlobal.left + enemyLocal.width, enemyGlobal.top + enemyLocal.height }, result) || // right of box
				line_intersects({ railX, railY }, { railX - 4000, railY }, { enemyGlobal.left, enemyGlobal.top + enemyLocal.height }, { enemyGlobal.left + enemyLocal.width, enemyGlobal.top + enemyLocal.height }, result)) // bottom of box
			{
				rails.push_back({});
				entityx::Entity railHit = entityManager.create();
				std::unique_ptr<sf::RectangleShape> rect(new sf::RectangleShape());
				rect->setSize({ result.x - railX, 5.f });
				rect->setPosition(_rail.pos.pos);
				rect->setFillColor(sf::Color::White);
				railHit.assign<cBasicRail>(
					std::move(rect),
					3,
					true);

				eventManager.emit<evHitEnemy>(entity, _rail.rail->damage);
			}
		}
	});
}

void weaponSystem::receive(const evFireMelee& melee)
{
	// loop through each enemy checking for a collision
	// later on use quadtrees
	entityManager.each<cEnemyType, cPosition, cRenderable, cAnimation>([this, melee](entityx::Entity entity, cEnemyType &type, cPosition &pos, cRenderable &render, cAnimation &animation)
	{
		sf::FloatRect mLocalBounds = melee.sprite->box->getLocalBounds();
		sf::FloatRect mGlobalBounds = melee.sprite->box->getGlobalBounds();
		sf::Vector2f mScale = melee.sprite->box->getScale();
		// if the player is on the left side of the enemy and we are shooting right (this is kinda assuming its only shooting straight forward)
		if (melee.pos.pos.x <= pos.pos.x && melee.dir.right)
		{
			sf::RectangleShape hitbox;
			hitbox.setSize(sf::Vector2f(mLocalBounds.width * mScale.x, mLocalBounds.height * mScale.y));
			hitbox.setOrigin(sf::Vector2f(hitbox.getSize().x / 2, hitbox.getSize().y / 2));
			hitbox.setPosition(sf::Vector2f(mGlobalBounds.left + ((mLocalBounds.width * fabs(mScale.x)) / 2), mGlobalBounds.top + ((mLocalBounds.height * fabs(mScale.y)) / 2)));
		
			if (hitbox.getGlobalBounds().intersects(render.box->getGlobalBounds()))
				eventManager.emit<evHitEnemy>(entity, melee.melee->damage);
		}
		// if the player is on the right side of the enemy and shooting left (same warning applies)
		else if (melee.pos.pos.x > pos.pos.x && !melee.dir.right)
		{
			sf::RectangleShape hitbox;
			hitbox.setSize(sf::Vector2f(mLocalBounds.width * mScale.x, mLocalBounds.height * mScale.y));
			hitbox.setOrigin(sf::Vector2f(hitbox.getSize().x / 2, hitbox.getSize().y / 2));
			hitbox.setPosition(sf::Vector2f(mGlobalBounds.left + ((mLocalBounds.width * fabs(mScale.x)) / 2), mGlobalBounds.top + ((mLocalBounds.height * fabs(mScale.y)) / 2)));

			if (hitbox.getGlobalBounds().intersects(render.box->getGlobalBounds()))
				eventManager.emit<evHitEnemy>(entity, melee.melee->damage);
		} // the code above is the same code from the if statement above it, maybe make it a function?
	});
}

// Taken from Graphics Gems 2, original author of lines_intersect: Mukesh Prasad. Link: http://www.realtimerendering.com/resources/GraphicsGems/gemsii/xlines.c 
bool weaponSystem::same_sign(float a, float b)
{
	return (((long)((unsigned long)a ^ (unsigned long)b)) >= 0);
}

/* p1 --- p2 = the first line
   p3 --- p4 = the second line
   collision = point of intersection

   Returns:
		0 = does not intersect
		1 = intersects
		2 = collinear
*/
int weaponSystem::line_intersects(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3, sf::Vector2f p4, sf::Vector2f& collision)
{
	float x1 = p1.x, y1 = p1.y;
	float x2 = p2.x, y2 = p2.y;
	float x3 = p3.x, y3 = p3.y;
	float x4 = p4.x, y4 = p4.y;

	float a1, a2, b1, b2, c1, c2;
	float r1, r2, r3, r4;
	float denom, offset, num;

	a1 = y2 - y1;
	b1 = x1 - x2;
	c1 = x2 * y1 - x1 * y2;

	r3 = a1 * x3 + b1 * y3 + c1;
	r4 = a1 * x4 + b1 * y4 + c1;

	if (r3 != 0 && r4 != 0 && same_sign(r3, r4))
		return 0; // don't intersect

	a2 = y4 - y3;
	b2 = x3 - x4;
	c2 = x4 * y3 - x3 * y4;

	r1 = a2 * x1 + b2 * y1 + c2;
	r2 = a2 * x2 + b2 * y2 + c2;

	if (r1 != 0 && r2 != 0 && same_sign(r1, r2))
		return 0; // don't intersect

	denom = a1 * b2 - a2 * b1;

	if (denom == 0)
		return 2; // collinear

	offset = denom < 0 ? -denom / 2 : denom / 2;

	num = b1 * c2 - b2 * c1;
	float x = (num < 0 ? num - offset : num + offset) / denom;

	num = a2 * c1 - a1 * c2;
	float y = (num < 0 ? num - offset : num + offset) / denom;

	collision = { x, y }; 

	return 1; // intersects
}