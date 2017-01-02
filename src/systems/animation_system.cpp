#include "animation_system.hpp"

animationSystem::animationSystem(entityx::EventManager& _eventManager) :
	eventManager(_eventManager)
{};

void animationSystem::configure(entityx::EventManager& eventManager)
{
	//eventManager.subscribe<evPlayerAnimationSet>(*this);
}

void animationSystem::update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt)
{
	entities.each<cAnimation, cRenderable>([](entityx::Entity entity, cAnimation &animation, cRenderable &render)
	{
		animation.animations.update();
		render.box->setTextureRect(animation.animations.getCurrentRect());
	});
}
