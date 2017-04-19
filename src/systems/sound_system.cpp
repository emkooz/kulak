#include "sound_system.hpp"

soundSystem::soundSystem(entityx::EntityManager& _entityManager, entityx::EventManager &_eventManager) :
	entityManager(_entityManager), eventManager(_eventManager)
{
};

void soundSystem::configure(entityx::EventManager& eventManager)
{
	eventManager.subscribe<evPlaySound>(*this);
	eventManager.subscribe<entityx::ComponentAddedEvent<cPlayerID>>(*this);
}

void soundSystem::update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt)
{
	entities.each<cPlayerID, cPosition>([this](entityx::Entity entity, cPlayerID& id, cPosition& pos)
	{
		// TODO: decide if wanting to include y coord. for stuff like enemies above or below a player, etc
		sf::Listener::setPosition(pos.pos.x, pos.pos.y, 0);
	});

	entities.each<cPosition, cSound>([this](entityx::Entity entity, cPosition& pos, cSound& sound)
	{
		if (sound.spatial)
		{
			for (auto x : sound.sounds)
				x->setPosition(pos.pos.x, pos.pos.y, 0);
		}
	});
}

void soundSystem::receive(const evPlaySound& event)
{
	int index = -1;
	for (int x = 0; x < event.sound->names.size(); x++)
	{
		if (event.sound->names[x] == event.name)
			index = x; // index in string vector needs to be same index for sound vector
	}

	if (index > -1)
	{
		event.sound->sounds[index]->play();
	}
	else
	{
		kk::log("Could not play:" + event.name, kk::logType::error);
	}
}

void soundSystem::receive(const entityx::ComponentAddedEvent<cPlayerID> &event)
{
	pEntity = event.entity;
}