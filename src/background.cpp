#include "background.hpp"

void background::load(entityx::EntityManager& entityManager, entityx::EventManager& eventManager)
{
	eBG = entityManager.create();
	eBG.assign<cPosition>(sf::Vector2f(0.f, 0.f));
	eBG.assign<cBackground>();
	std::shared_ptr<sf::Sprite> bgSprite(new sf::Sprite);
	bgSprite->setTexture(*kk::getTexture("menubg"));
	bgSprite->setOrigin(bgSprite->getTexture()->getSize().x / 2, bgSprite->getTexture()->getSize().y / 2);
	bgSprite->setPosition(0.f, 0.f);
	//bgSprite->setColor(sf::Color::Transparent);
	bounds = bgSprite->getGlobalBounds();
	eBG.assign<cRenderable>(
		bgSprite,
		0, // renderlayer is always 0 for bg
		true);

	eventManager.subscribe<evChangeBackground>(*this);
	eventManager.emit<evBackgroundCreated>(this);
}

void background::receive(const evChangeBackground &event)
{
	entityx::ComponentHandle<cRenderable> bg = eBG.component<cRenderable>();
	bg->box->setTexture(*kk::getTexture(event.name));
	bg->box->setOrigin(bg->box->getTexture()->getSize().x / 2, bg->box->getTexture()->getSize().y / 2);
	bounds = bg->box->getGlobalBounds();
}

sf::FloatRect background::getBounds()
{
	return bounds;
}