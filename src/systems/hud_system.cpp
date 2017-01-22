#include "hud_system.hpp"

hudSystem::hudSystem(entityx::EntityManager& _entityManager, entityx::EventManager &_eventManager, sf::RenderWindow* _window) :
	entityManager(_entityManager), eventManager(_eventManager), window(_window)
{
	//cameraView.setCenter(0, 0);
};

void hudSystem::configure(entityx::EventManager& eventManager)
{
	eventManager.subscribe<evSwitchWeapon>(*this);
	eventManager.subscribe<evStatsCreated>(*this);
}

void hudSystem::update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt)
{
	hpText.component<cRenderableTextHUD>()->text->setString(std::to_string(pStats->getHealth()));
	goldText.component<cRenderableTextHUD>()->text->setString(std::to_string(pStats->getGold()));
}

void hudSystem::receive(const evStatsCreated &event)
{
	pStats = event.pStats;
	hpTexture = entityManager.create(); hpTexture.assign<cStaticView>();
	hpText = entityManager.create(); hpText.assign<cStaticView>();
	goldTexture = entityManager.create(); goldTexture.assign<cStaticView>();
	goldText = entityManager.create(); goldText.assign<cStaticView>();
	rail = entityManager.create(); rail.assign<cStaticView>(); // this will be made dynamic later
	melee = entityManager.create(); melee.assign<cStaticView>(); // this will be made dynamic later
	selectedWeapon = entityManager.create(); selectedWeapon.assign<cStaticView>();

	// Setup scene here.
	window->setView(window->getDefaultView());
	// HP texture and text
	std::shared_ptr<sf::Sprite> hpBox(new sf::Sprite);
	hpBox->setTexture(*kk::getTexture("health"));
	hpBox->setPosition(4, window->getSize().y - (hpBox->getTexture()->getSize().y + 4)); // positioned at 4, size - 4 for whatever screen size
	hpTexture.assign<cRenderableHUD>(
		hpBox,
		0, // HUD is the lowest layer of UI
		true);

	std::shared_ptr<sf::Text> _hpText(new sf::Text);
	_hpText->setFont(*kk::getFont("font"));
	_hpText->setString(std::to_string(pStats->getHealth()));
	_hpText->setPosition(hpBox->getPosition().x + hpBox->getTexture()->getSize().x + 2, hpBox->getPosition().y + (hpBox->getTexture()->getSize().y / 4));
	_hpText->setColor(sf::Color::White);
	_hpText->setCharacterSize(24);
	hpText.assign<cRenderableTextHUD>(
		_hpText,
		0,
		true);

	// Gold texture and text
	std::shared_ptr<sf::Sprite> goldBox(new sf::Sprite);
	goldBox->setTexture(*kk::getTexture("coin"));
	goldBox->setPosition(_hpText->getPosition().x + _hpText->getGlobalBounds().width + 15, window->getSize().y - (goldBox->getTexture()->getSize().y + 4)); // positioned at 4, size - 4 for whatever screen size
	goldTexture.assign<cRenderableHUD>(
		goldBox,
		0, // HUD is the lowest layer of UI
		true);

	std::shared_ptr<sf::Text> _goldText(new sf::Text);
	_goldText->setFont(*kk::getFont("font"));
	_goldText->setString(std::to_string(pStats->getGold()));
	_goldText->setPosition(goldBox->getPosition().x + goldBox->getTexture()->getSize().x + 2, goldBox->getPosition().y + (goldBox->getTexture()->getSize().y / 4));
	_goldText->setColor(sf::Color::White);
	_goldText->setCharacterSize(24);
	goldText.assign<cRenderableTextHUD>(
		_goldText,
		0,
		true);

	//TODO FUTURE: current weapons. right now its just always going to be rail + melee. make this dynamic
	std::shared_ptr<sf::Sprite> railBox(new sf::Sprite);
	railBox->setTexture(*kk::getTexture("ak"));
	railBox->setScale(0.25, 0.25);
	railBox->setPosition((window->getSize().x / 2) - (railBox->getTexture()->getSize().x * railBox->getScale().x), window->getSize().y - (railBox->getTexture()->getSize().y * railBox->getScale().y) - 2);
	rail.assign<cRenderableHUD>(
		railBox,
		0,
		true);

	std::shared_ptr<sf::Sprite> meleeBox(new sf::Sprite);
	meleeBox->setTexture(*kk::getTexture("knife"));
	meleeBox->setScale(0.25, 0.25);
	meleeBox->setPosition((window->getSize().x / 2) + 6, window->getSize().y - (meleeBox->getTexture()->getSize().y * meleeBox->getScale().y) - 2);
	melee.assign<cRenderableHUD>(
		meleeBox,
		0,
		true);

	// this is for the selected weapon box
	std::shared_ptr<sf::RectangleShape> selectedBox(new sf::RectangleShape);
	selectedBox->setFillColor(sf::Color::Transparent);
	selectedBox->setSize(sf::Vector2f(railBox->getTexture()->getSize().x * 0.25, railBox->getTexture()->getSize().y * 0.25));
	selectedBox->setPosition(railBox->getPosition());
	selectedBox->setOutlineThickness(2);
	selectedBox->setOutlineColor(sf::Color::White);
	selectedWeapon.assign<cRenderableRectHUD>(
		selectedBox,
		0,
		true);
}

void hudSystem::receive(const evSwitchWeapon &event)
{
	// looks ugly af
	entityx::ComponentHandle<cRenderableHUD> baseWeapon = rail.component<cRenderableHUD>();
	selectedWeapon.component<cRenderableRectHUD>()->rect->setPosition(
		(baseWeapon->box->getPosition().x) + (baseWeapon->box->getTexture()->getSize().x * baseWeapon->box->getScale().x * event.weapon) + (6 * event.weapon),
		baseWeapon->box->getPosition().y);
}