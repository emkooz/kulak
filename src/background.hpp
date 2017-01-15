#pragma once
#include <entityx/entityx.h>
#include <SFML/Graphics.hpp>
#include "components/cPlayer.hpp"
#include "resource.hpp"


class background
{
public:
	void load(entityx::EntityManager& entityManager, entityx::EventManager& event_manager);
	sf::FloatRect getBounds();
private:
	entityx::Entity eBG; // background entity
	sf::FloatRect bounds;
};

struct evBackgroundCreated
{
	evBackgroundCreated(background* bg) : bg(bg) {}
	background* bg;
};
