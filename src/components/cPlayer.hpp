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

struct health
{
	health(int hp = 100) : hp(hp) {}
	int hp;
};

struct playerID
{
	playerID(int id = 0) : id(id) {}
	int id;
};

// Physics
struct position
{
	position(sf::Vector2f pos = sf::Vector2f(0.0f, 0.0f)) : pos(pos) {}
	sf::Vector2f pos;
};

struct direction
{
	direction(sf::Vector2f angle = sf::Vector2f(0.0f, 0.0f)) : angle(angle) {}
	sf::Vector2f angle;
};

struct velocity
{
	velocity(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}
	float x, y;
};

//Render
struct renderable
{
	renderable(
		std::unique_ptr<sf::Sprite> box,
		int renderLayer = 0,
		bool render = true
	) : box(std::move(box)), renderLayer(renderLayer), render(render) {}
	std::unique_ptr<sf::Sprite> box;
	int renderLayer; // change to enum later
	bool render;
};