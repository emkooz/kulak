#pragma once
#include <SFML/System/Clock.hpp>

struct cEnemyType
{
	cEnemyType(int type = 0) : type(type), hitVisualDuration(0.075) {}
	int type; // make enum later
	float hitVisualDuration;
	sf::Clock hitDurationClock;
};