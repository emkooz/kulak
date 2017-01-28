#pragma once
#include <entityx/entityx.h>
#include <SFML/Graphics.hpp>
#include "components/cPlayer.hpp"
#include "components/cRender.hpp"
#include "resource.hpp"
#include "components/events.hpp"


class background : public entityx::Receiver<background>
{
public:
	void load(entityx::EntityManager& entityManager, entityx::EventManager& event_manager);
	void receive(const evChangeBackground &event);
	sf::FloatRect getBounds();
private:
	entityx::Entity eBG; // background entity
	sf::FloatRect bounds;
};

