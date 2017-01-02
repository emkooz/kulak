#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>
#include <memory>

/* Components that a player requires:
health
player id
mana?
Physics:
	position
	direction
	velocity
Render:
	renderable (sf::RectangleShape, sf::Texture/animation class later, int render_layer, bool render)
*/

struct cHealth
{
	cHealth(int hp = 100) : hp(hp) {}
	int hp;
};

struct cPlayerID
{
	cPlayerID(int id = 0) : id(id) {}
	int id;
};

// Physics
struct cPosition
{
	cPosition(sf::Vector2f pos = sf::Vector2f(0.0f, 0.0f)) : pos(pos) {}
	sf::Vector2f pos;
};

struct cDirection
{
	cDirection(sf::Vector2f angle = sf::Vector2f(0.0f, 0.0f)) : angle(angle) {}
	sf::Vector2f angle;
};

struct cVelocity
{
	cVelocity(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}
	float x, y;
};

//Render
struct cRenderable
{
	cRenderable(
		std::unique_ptr<sf::Sprite> box,
		int renderLayer = 0,
		bool render = true
	) : box(std::move(box)), renderLayer(renderLayer), render(render) {}
	std::unique_ptr<sf::Sprite> box;
	int renderLayer; // change to enum later
	bool render;
};