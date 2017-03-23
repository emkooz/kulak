#include "menu.hpp"

menuSystem::menuSystem(entityx::EntityManager& _entityManager, entityx::EventManager &_eventManager, sf::RenderWindow* _window) :
	entityManager(_entityManager), eventManager(_eventManager), window(_window), mousePressed(false)
{
	playText = entityManager.create(); playText.assign<cStaticView>();
	optionsText = entityManager.create(); optionsText.assign<cStaticView>();
	quitText = entityManager.create(); quitText.assign<cStaticView>();

	std::shared_ptr<sf::Text> _playText(new sf::Text);
	_playText->setFont(*kk::getFont("font"));
	_playText->setString("Play");
	_playText->setColor(sf::Color::White);
	_playText->setCharacterSize(32);
	_playText->setPosition(4, window->getSize().y / 2);
	playText.assign<cRenderableMenuText>(
		_playText,
		0,
		true);

	std::shared_ptr<sf::Text> _optionsText(new sf::Text);
	_optionsText->setFont(*kk::getFont("font"));
	_optionsText->setString("Options");
	_optionsText->setColor(sf::Color::White);
	_optionsText->setCharacterSize(32);
	_optionsText->setPosition(4, _playText->getPosition().y + _playText->getLocalBounds().height + 10);
	optionsText.assign<cRenderableMenuText>(
		_optionsText,
		0,
		true);

	std::shared_ptr<sf::Text> _quitText(new sf::Text);
	_quitText->setFont(*kk::getFont("font"));
	_quitText->setString("Quit");
	_quitText->setColor(sf::Color::White);
	_quitText->setCharacterSize(32);
	_quitText->setPosition(4, _optionsText->getPosition().y + _optionsText->getLocalBounds().height + 10);
	quitText.assign<cRenderableMenuText>(
		_quitText,
		0,
		true);
};

void menuSystem::configure(entityx::EventManager& eventManager)
{
}

void menuSystem::update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt)
{
	//hpText.component<cRenderableTextHUD>()->text->setString(std::to_string(pStats->getHealth()));
	entityx::ComponentHandle<cRenderableMenuText> play = playText.component<cRenderableMenuText>();
	entityx::ComponentHandle<cRenderableMenuText> options = optionsText.component<cRenderableMenuText>();
	entityx::ComponentHandle<cRenderableMenuText> quit = quitText.component<cRenderableMenuText>();

	// hovering over a menu item
	// mouse is inside text
	if (play->text->getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
	{
		if (!play->selected)
		{
			play->text->setColor(sf::Color::Red);
			play->selected = true;
		}

		if (!mousePressed && sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			events.emit<evChangeBackground>("bg");
			events.emit<evSetState>(kk::gameState::STATE_PREGAME);
			mousePressed = true;
		}
	}
	// mouse is outside text but selected is set to true
	else if (!play->text->getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))) && play->selected)
	{
		play->text->setColor(sf::Color::White);
		play->selected = false;
	}
	
	if (quit->text->getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
	{
		if (!quit->selected)
		{
			quit->text->setColor(sf::Color::Red);
			quit->selected = true;
		}

		if (!mousePressed && sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			events.emit<evSetState>(kk::gameState::STATE_QUIT);
			mousePressed = true;
		}
	}
	else if (!quit->text->getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))) && quit->selected)
	{
		quit->text->setColor(sf::Color::White);
		quit->selected = false;

	}
	// TODO: real options
	if (options->text->getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))))
	{
		if (!options->selected)
		{
			options->text->setColor(sf::Color::Red);
			options->selected = true;
		}
	}
	else if (!options->text->getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window))) && options->selected)
	{
		options->text->setColor(sf::Color::White);
		options->selected = false;
	}

	if (mousePressed && !sf::Mouse::isButtonPressed(sf::Mouse::Left))
		mousePressed = false;

}