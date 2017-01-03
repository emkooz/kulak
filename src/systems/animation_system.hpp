#pragma once
#include <string>
#include <entityx/entityx.h>
#include "components/cAnimation.hpp"
#include "components/cPlayer.hpp"

struct evPlayerAnimationSet
{
	evPlayerAnimationSet(int id, bool right, std::string name) : id(id), right(right), name(name) {}
	int id; // player id
	bool right; // facing the right direction or left (left == reversed)
	std::string name; // name of animation to change to
};

class animationSystem : public entityx::System<animationSystem>, public entityx::Receiver<animationSystem>
{
public:
	animationSystem(entityx::EntityManager& entityManager);
	void configure(entityx::EventManager& eventManager);
	void update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt);
	void receive(const evPlayerAnimationSet& animation);
	void receive(const entityx::ComponentAddedEvent<cAnimation> &event);
private:
	entityx::EntityManager& entityManager;
};