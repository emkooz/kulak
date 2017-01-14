#pragma once
#include <entityx/entityx.h>
#include <SFML/Graphics.hpp>
#include "components/cPlayer.hpp"

class cameraSystem : public entityx::System<cameraSystem>, public entityx::Receiver<cameraSystem>
{
public:
	cameraSystem(entityx::EntityManager& entityManager, sf::RenderWindow* window);
	void configure(entityx::EventManager& eventManager);
	void update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt);
	void receive(const entityx::ComponentAddedEvent<cPlayerID> &event);
private:
	entityx::EntityManager& entityManager;
	entityx::Entity pEntity;
	sf::RenderWindow* window;
	sf::View cameraView;
};