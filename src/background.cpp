#include "background.hpp"

void background::load(entityx::EntityManager& entityManager, entityx::EventManager& eventManager)
{
	eBG = entityManager.create();
	eBG.assign<cPosition>(sf::Vector2f(0.f, 0.f));
	std::unique_ptr<sf::Sprite> bgSprite(new sf::Sprite);
	bgSprite->setTexture(*kk::getTexture("bg"));
	bgSprite->setOrigin(bgSprite->getTexture()->getSize().x / 2, bgSprite->getTexture()->getSize().y / 2);
	bgSprite->setPosition(0.f, 0.f);
	bounds = bgSprite->getGlobalBounds();
	eBG.assign<cRenderable>(
		std::move(bgSprite),
		0, // renderlayer is always 0 for bg
		true);

	eventManager.emit<evBackgroundCreated>(this);
}

sf::FloatRect background::getBounds()
{
	return bounds;
}