#include "camera.hpp"

cameraSystem::cameraSystem(entityx::EntityManager& _entityManager, sf::RenderWindow* _window) :
	entityManager(_entityManager), window(_window)
{
	cameraView.setCenter(0, 0);
};

void cameraSystem::configure(entityx::EventManager& eventManager)
{
	eventManager.subscribe<entityx::ComponentAddedEvent<cPlayerID>>(*this);
	eventManager.subscribe<evBackgroundCreated>(*this);
}

void cameraSystem::update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt)
{
	if (pEntity.valid())
	{
		sf::Vector2f pos = pEntity.component<cRenderable>()->box->getPosition();
		sf::FloatRect bounds = bg->getBounds();

		if ((cameraView.getCenter().x + deadzone) < pos.x) // right of deadzone
		{
			// if right edge of camera is left of background edge
			if ((cameraView.getCenter().x + (cameraView.getSize().x / 2)) < bounds.left + bounds.width)
				cameraView.setCenter({ pos.x - deadzone, 0 });
			else // else set it to the very edge of the background on the right
				cameraView.setCenter({(bounds.left + bounds.width) - (cameraView.getSize().x / 2) , 0});
		}
		else if ((cameraView.getCenter().x - deadzone) > pos.x) // left of deadzone
		{
			// same as above but for the left side
			if ((cameraView.getCenter().x - (cameraView.getSize().x / 2)) > bounds.left)
				cameraView.setCenter({ pos.x + deadzone, 0 });
			else
				cameraView.setCenter({bounds.left + (cameraView.getSize().x / 2), 0});
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

void cameraSystem::receive(const evBackgroundCreated &event)
{
	bg = event.bg;
}