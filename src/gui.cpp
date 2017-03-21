#include "gui.hpp"

GUI::GUI(entityx::EntityManager& entityManager, entityx::EventManager& eventManager) :
	entityManager(entityManager), eventManager(eventManager)
{
	eventManager.subscribe<evSetState>(*this);
};

void GUI::init(sf::RenderWindow* _window)
{
	gui.setWindow(*_window);
	kk::log("Setting up GUI...");

	auto width = tgui::bindWidth(gui);
	auto height = tgui::bindHeight(gui);

	// Stats widgets
	auto bPlay = tgui::Button::create();
	bPlay->setSize(width * 0.1, height * 0.1);
	bPlay->setPosition(centerHorizontal(bPlay, width), height * 0.8);
	bPlay->setText("Play");
	bPlay->hide();
	bPlay->connect("pressed", [&](){eventManager.emit<evSetState>(kk::STATE_PLAYING);});
	gui.add(bPlay, "bStatPlay");
	statWidgets.emplace_back(bPlay);
}

void GUI::configure(entityx::EventManager& eventManager)
{
}

void GUI::receive(const evSetState& event)
{
	// handle state being changed, hide/show applicable elements
	auto currentState = kk::getState();

	// loop through to hide elements
	if (currentState == kk::STATE_PREGAME)
	{
		for (auto x : statWidgets)
			x->hide();
	}

	// loop through to show elements
	if (event.state == kk::STATE_PREGAME)
	{
		for (auto x : statWidgets)
			x->show();
	}
}

void GUI::pollEvents(sf::Event events)
{
	gui.handleEvent(events);
}

void GUI::draw()
{
	gui.draw();
}

template <typename T>
tgui::Layout GUI::centerHorizontal(T widget, tgui::Layout width)
{
	return ((width / 2) - (widget->getSize().x / 2));
}

template <typename T>
tgui::Layout GUI::centerVertical(T widget, tgui::Layout height)
{
	return ((height / 2) - (widget->getSize().y / 2));
}