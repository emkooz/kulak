#include "camera.hpp"

cameraSystem::cameraSystem(entityx::EntityManager& _entityManager, sf::RenderWindow* _window) :
	entityManager(_entityManager), window(_window)
{
	cameraView.setCenter(0, 0);
};

void cameraSystem::configure(entityx::EventManager& eventManager)
{
	eventManager.subscribe<entityx::ComponentAddedEvent<cPlayerID>>(*this);
}

void cameraSystem::update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt)
{
	if (pEntity.valid())
	{
		sf::Vector2f pos = pEntity.component<cRenderable>()->box->getPosition();

		if ((cameraView.getCenter().x + deadzone) < pos.x) // right of deadzone
		{
			cameraView.setCenter({ pos.x - deadzone, 0 });
		}
		else if ((cameraView.getCenter().x - deadzone) > pos.x) // left of deadzone
		{
			cameraView.setCenter({ pos.x + deadzone, 0 });
		}

		window->setView(cameraView);
	}
}

void cameraSystem::receive(const entityx::ComponentAddedEvent<cPlayerID> &event)
{
	pEntity = event.entity;
	cameraView.setSize(sf::Vector2f(window->getSize()));
	window->setView(cameraView);

	deadzone = window->getSize().x * 0.05; // deadzone is 5% (x and -x) of width
}