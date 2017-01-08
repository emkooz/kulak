#pragma once
#include <SFML/Graphics.hpp>

struct cRail
{
	// have stuff like distance, position, damage, etc
	cRail(sf::Vector2f pos) : pos(pos) {}
	sf::Vector2f pos;
};