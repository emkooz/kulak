#include "animation_system.hpp"

animationSystem::animationSystem(entityx::EntityManager& _entityManager) :
	entityManager(_entityManager)
{};

void animationSystem::configure(entityx::EventManager& eventManager)
{
	eventManager.subscribe<evPlayerAnimationSet>(*this);
	eventManager.subscribe<evAddedLayerToAnimation>(*this);
	eventManager.subscribe<entityx::ComponentAddedEvent<cAnimation>>(*this);
}

void animationSystem::update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt)
{
	entities.each<cAnimation, cRenderable>([](entityx::Entity entity, cAnimation &animation, cRenderable &render)
	{
		animation.animations.update();
		render.box->setTextureRect(animation.animations.getCurrentRect());
	});

	entities.each<cAnimationLayered, cRenderable>([](entityx::Entity entity, cAnimationLayered &animation, cRenderable &render)
	{
		for (int x = 0; x < animation.animations.size(); x++)
		{
			animation.animations[x].update();

			if (x > animation.entityLayer)
			{
				animation.otherLayers[x - 1].box->setTextureRect(animation.animations[x].getCurrentRect());
				// change position for each connected layer relative to main entity layer (the body)
				animation.otherLayers[x - 1].box->setPosition(render.box->getPosition());
			}
			else if (x < animation.entityLayer)
			{
				animation.otherLayers[x].box->setTextureRect(animation.animations[x].getCurrentRect());
				animation.otherLayers[x].box->setPosition(render.box->getPosition());
			}
		}

		render.box->setTextureRect(animation.animations[animation.entityLayer].getCurrentRect());
	});
}

void animationSystem::receive(const evPlayerAnimationSet& evAnim)
{
	entityManager.each<cPlayerID, cAnimationLayered, cRenderable, cCurrentWeapon>([evAnim](entityx::Entity entity, cPlayerID &id, cAnimationLayered &animation, cRenderable &render, cCurrentWeapon &weapon)
	{
		if (id.id == evAnim.id)
		{
			animation.state = evAnim.name;

			auto anim = animation.otherLayers;

			for (int x = 0; x < animation.animations.size(); x++)
			{
				animation.animations[x].setAnimation(evAnim.name, evAnim.right);
				
				if (x <= animation.entityLayer)
				{
					animation.animations[x].setReversed(render.box, evAnim.right);
				}
				else if (x > animation.entityLayer)
				{
					animation.animations[x - 1].setReversed(render.box, evAnim.right);
				}
			}
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

void animationSystem::receive(const evAddedLayerToAnimation &event)
{
	auto animation = event.ent.component<cAnimationLayered>();
	auto ent = event.ent;

	if (event.layer < animation->entityLayer)
		animation->otherLayers[event.layer].box->setOrigin({ (float)animation->animations[event.layer].getSize().x / 2, (float)animation->animations[event.layer].getSize().y / 2 });
	else if (event.layer > animation->entityLayer)
		animation->otherLayers[event.layer-1].box->setOrigin({ (float)animation->animations[event.layer-1].getSize().x / 2, (float)animation->animations[event.layer-1].getSize().y / 2 });
	else
		ent.component<cRenderable>()->box->setOrigin({ (float)animation->animations[event.layer].getSize().x / 2, (float)animation->animations[event.layer].getSize().y / 2 });
}