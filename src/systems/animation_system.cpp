#include "animation_system.hpp"

animationSystem::animationSystem(entityx::EntityManager& _entityManager) :
	entityManager(_entityManager)
{};

void animationSystem::configure(entityx::EventManager& eventManager)
{
	eventManager.subscribe<evPlayerAnimationSet>(*this);
	eventManager.subscribe<entityx::ComponentAddedEvent<cAnimation>>(*this);
}

void animationSystem::update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt)
{
	entities.each<cAnimation, cRenderable>([](entityx::Entity entity, cAnimation &animation, cRenderable &render)
	{
		animation.animations.update();
		render.box->setTextureRect(animation.animations.getCurrentRect());
	});
}

void animationSystem::receive(const evPlayerAnimationSet& evAnim)
{
	entityManager.each<cPlayerID, cAnimation, cRenderable>([evAnim](entityx::Entity entity, cPlayerID &id, cAnimation &animation, cRenderable &render)
	{
		if (id.id == evAnim.id)
		{
			//animation.animations.setAnimation(evAnim.name);
			if (evAnim.right)
				render.box->setScale({ 1, 1 });
			else
				render.box->setScale({ -1, 1 }); // flip if facing left, default facing right

			animation.animations.setAnimation(evAnim.name, evAnim.right);
		}
	});
}

void animationSystem::receive(const entityx::ComponentAddedEvent<cAnimation> &event)
{
	auto animation = event.component;
	auto ent = event.entity;
	// when adding animation to a sprite, set the origin of the sprite to the middle of each frame
	ent.component<cRenderable>()->box->setOrigin({ (float)animation->animations.getSize().x / 2, (float)animation->animations.getSize().y / 2 });
}