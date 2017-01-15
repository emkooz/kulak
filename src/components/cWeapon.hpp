#pragma once
#include <SFML/Graphics.hpp>

struct cRail
{
	// have stuff like distance, position, damage, etc
	cRail(sf::Vector2f pos, float cooldown) : pos(pos), cooldown(cooldown) {}
	sf::Vector2f pos;
	float cooldown; // delay per shot in seconds
	sf::Clock cooldownTimer;
};