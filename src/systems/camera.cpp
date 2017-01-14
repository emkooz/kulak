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
		cameraView.setCenter(sf::Vector2f(pEntity.component<cRenderable>()->box->getPosition().x, 0));
		window->setView(cameraView);
	}
}

void cameraSystem::receive(const entityx::ComponentAddedEvent<cPlayerID> &event)
{
	pEntity = event.entity;
	cameraView.setSize(sf::Vector2f(window->getSize()));
	window->setView(cameraView);
}