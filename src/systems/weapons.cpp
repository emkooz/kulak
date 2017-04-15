#include "weapons.hpp"

weaponSystem::weaponSystem(entityx::EntityManager& _entityManager, entityx::EventManager& _eventManager) :
	entityManager(_entityManager), eventManager(_eventManager)
{};

void weaponSystem::configure(entityx::EventManager& eventManager)
{
	eventManager.subscribe<evFireRail>(*this);
	eventManager.subscribe<evFireMelee>(*this);
	eventManager.subscribe<evFireProjectile>(*this);
	eventManager.subscribe<evBackgroundCreated>(*this);
}

void weaponSystem::update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt)
{
	sf::FloatRect bounds = bg->getBounds();

	// update projectiles
	entityManager.each<cProjectile, cVelocity, cDirection, cPosition, cRenderable, cWeaponBase>([this, &dt, &bounds](entityx::Entity entity, cProjectile &proj, cVelocity &vel, 
																										cDirection &dir, cPosition &pos, cRenderable &render, cWeaponBase &base)
	{
		if (proj.owner == "player") // only checking projectiles that came from the player
		{
			pos.pos.x += ((dir.right ? 1 : -1) * (vel.velocity.x * dt));
			pos.pos.y += vel.velocity.y * dt;
			render.box->setPosition(pos.pos);

			// check collision between each enemy (use quadtree later)
			entityManager.each<cEnemyType, cPosition, cRenderable, cAnimation>([this, &render, &base, &entity](entityx::Entity _entity, cEnemyType &_type, cPosition &_pos, cRenderable &_render, cAnimation &_animation)
			{
				if (render.box->getGlobalBounds().intersects(_render.box->getGlobalBounds()))
				{
					eventManager.emit<evHitEnemy>(_entity, base.damage);
					entity.destroy();
				}
			});

			// kill projectile if it reaches the edges of the map + 50?
			if ((pos.pos.x > bounds.left + bounds.width + 50) || (pos.pos.x < bounds.left - 50))
				entity.destroy();
		}
	});
}

void weaponSystem::receive(const evFireRail& _rail)
{
	// loop through each enemy checking for a collision
	// later on use quadtrees
	// TODO: set tracer to an actual texture/animation
	entityx::Entity closestEnemyHit;
	sf::Vector2f hitLocation;

	entityManager.each<cEnemyType, cPosition, cRenderable>([this, _rail, &closestEnemyHit, &hitLocation](entityx::Entity entity, cEnemyType &type, cPosition &pos, cRenderable &render)
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
				if (closestEnemyHit == NULL)
				{
					closestEnemyHit = entity; 
					hitLocation = result;
				}
				else if (std::abs(closestEnemyHit.component<cPosition>()->pos.x - railX) > std::abs(pos.pos.x - railX)) // if the current enemy is closer to the player than the last closest enemy
				{
					closestEnemyHit = entity; 
					hitLocation = result;
				}

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
				if (closestEnemyHit == NULL)
				{
					closestEnemyHit = entity;
					hitLocation = result;
				}
				else if (std::abs(closestEnemyHit.component<cPosition>()->pos.x - railX) > std::abs(pos.pos.x - railX)) // if the current enemy is closer to the player than the last closest enemy
				{
					closestEnemyHit = entity;
					hitLocation = result;
				}
			}
		}
	});

	if (closestEnemyHit != NULL) // hit an enemy, send out enemy hit event
	{
		rails.push_back({});
		entityx::Entity railHit = entityManager.create();
		std::shared_ptr<sf::RectangleShape> rect(new sf::RectangleShape());
		rect->setSize({ hitLocation.x - _rail.pos.pos.x, 1.f });
		rect->setPosition(_rail.pos.pos);
		rect->setFillColor(sf::Color::Yellow);
		railHit.assign<cBasicRail>(
			rect,
			3,
			true);

		eventManager.emit<evHitEnemy>(closestEnemyHit, _rail.rail->damage);
	}
}

void weaponSystem::receive(const evFireMelee& melee)
{
	auto hitbox = melee.hitbox->hitbox;

	// loop through each enemy checking for a collision
	// later on use quadtrees
	entityManager.each<cEnemyType, cPosition, cRenderable, cAnimation>([this, melee, hitbox](entityx::Entity entity, cEnemyType &type, cPosition &pos, cRenderable &render, cAnimation &animation)
	{
		// if the player is on the left side of the enemy and we are shooting right (this is kinda assuming its only shooting straight forward)
		if (melee.pos.pos.x <= pos.pos.x && melee.dir.right)
		{
			if (hitbox.intersects(render.box->getGlobalBounds()))
				eventManager.emit<evHitEnemy>(entity, melee.melee->damage);
		}
		// if the player is on the right side of the enemy and shooting left (same warning applies)
		else if (melee.pos.pos.x > pos.pos.x && !melee.dir.right)
		{
			if (hitbox.intersects(render.box->getGlobalBounds()))
				eventManager.emit<evHitEnemy>(entity, melee.melee->damage);
		} // the code above is the same code from the if statement above it, maybe make it a function?
	});
}

void weaponSystem::receive(const evFireProjectile& weapon)
{
	// spawn projectile
	entityx::Entity projectile = entityManager.create();
	projectile.assign<cPosition>(sf::Vector2f(
		(weapon.dir ? weapon.pos->pos.x + weapon.hitbox->offset.x : weapon.pos->pos.x - weapon.hitbox->offset.x),
		 weapon.pos->pos.y));
	projectile.assign<cVelocity>(weapon.weapon->velocity);
	projectile.assign<cDirection>(weapon.dir->right);
	projectile.assign<cProjectile>("player");

	std::shared_ptr<sf::Sprite> sprite(new sf::Sprite());
	sprite->setTexture(*kk::getTexture("coin"));
	sprite->setPosition(projectile.component<cPosition>()->pos);
	sprite->setScale(0.1f, 0.1f);
	projectile.assign<cRenderable>(
		sprite,
		3,
		true
		);

	projectile.assign<cProjectileOrigin>(true); // from a player
	projectile.assign<cWeaponBase>(weapon.weapon);
}

void weaponSystem::receive(const evBackgroundCreated &event)
{
	bg = event.bg;
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