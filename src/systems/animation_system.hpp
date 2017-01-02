#pragma once
#include <string>
#include <entityx/entityx.h>
#include "components/cAnimation.hpp"
#include "components/cPlayer.hpp"

struct evPlayerAnimationSet
{
	evPlayerAnimationSet(int id, std::string name) : id(id), name(name) {}
	int id;
	std::string name;
};

class animationSystem : public entityx::System<animationSystem>, public entityx::Receiver<animationSystem>
{
public:
	animationSystem(entityx::EventManager& _eventManager);
	void configure(entityx::EventManager& eventManager);
	void update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt);
	//void receive(const evPlayerAnimationSet& animation);
private:
	entityx::EventManager& eventManager;
};