#include "gui.hpp"

GUI::GUI(entityx::EntityManager& entityManager, entityx::EventManager& eventManager) :
	entityManager(entityManager), eventManager(eventManager)
{
	eventManager.subscribe<evSetState>(*this);
	eventManager.subscribe<evStatsCreated>(*this);
};

void GUI::init(sf::RenderWindow* _window)
{
	gui.setWindow(*_window);
	kk::log("Setting up GUI...");

	auto width = tgui::bindWidth(gui);
	auto height = tgui::bindHeight(gui);

	// TODO: update tgui to dev branch, change labels to richtext?

	// Stats widgets //
	// Play button
	auto bPlay = tgui::Button::create();
		bPlay->setSize(width * 0.1, height * 0.1);
		bPlay->setPosition(centerHorizontal(bPlay, width), height * 0.8);
		bPlay->setText("Play");
		bPlay->hide();
		bPlay->connect("pressed", [&](){eventManager.emit<evSetState>(kk::STATE_PLAYING);});
		gui.add(bPlay, "bStatPlay");
		statWidgets.emplace_back(bPlay);

	// Gold text + image at top right
	auto tGold = tgui::Label::create();
		tGold->setText(std::to_string(pStats->getGold()));
		tGold->setTextSize(24);
		tGold->setPosition(width - 100, 2);
		tGold->hide();
		gui.add(tGold, "tStatGold");
		statWidgets.emplace_back(tGold);

	auto pGold = tgui::Picture::create();
		pGold->setTexture(*kk::getTexture("coin"));
		pGold->setPosition(tGold->getPosition().x - 30, 2);
		pGold->setSize(24, 24);
		pGold->hide();
		gui.add(pGold, "pStatGold");
		statWidgets.emplace_back(pGold);

	// Health text + image at top right
	auto tHealth = tgui::Label::create();
		tHealth->setText(std::to_string(pStats->getHealth()));
		tHealth->setTextSize(24);
		tHealth->setPosition(pGold->getPosition().x - 100, 2);
		tHealth->hide();
		gui.add(tHealth, "tStatHealth");
		statWidgets.emplace_back(tHealth);

	auto pHealth = tgui::Picture::create();
		pHealth->setTexture(*kk::getTexture("health"));
		pHealth->setPosition(tHealth->getPosition().x - 30, 2);
		pHealth->setSize(24, 24);
		pHealth->hide();
		gui.add(pHealth, "pStatHealth");
		statWidgets.emplace_back(pHealth);

	// Gold text + upgrade in middle
	auto tGoldUpgrade = tgui::Label::copy(tGold);
		tGoldUpgrade->setPosition(centerHorizontal(tGoldUpgrade, width) - 50, centerVertical(tGoldUpgrade, height));
		gui.add(tGoldUpgrade, "tStatGoldUpgrade");
		statWidgets.emplace_back(tGoldUpgrade);

	auto bGoldUpgrade = tgui::Button::create();
		bGoldUpgrade->setText("Upgrade");
		bGoldUpgrade->setPosition(tGoldUpgrade->getPosition().x + 100, centerVertical(bGoldUpgrade, height) + 2);
		bGoldUpgrade->setSize(75, 24);
		bGoldUpgrade->hide();
		bGoldUpgrade->connect("pressed", [&]() {eventManager.emit<evBuyMana>();});
		gui.add(bGoldUpgrade, "bStatGoldUpgrade");
		statWidgets.emplace_back(bGoldUpgrade);

	// Health text + upgrade in middle
	auto tHealthUpgrade = tgui::Label::copy(tHealth);
		tHealthUpgrade->setPosition(tGoldUpgrade->getPosition().x, tGoldUpgrade->getPosition().y + tGoldUpgrade->getSize().y);
		gui.add(tHealthUpgrade, "tStatHealthUpgrade");
		statWidgets.emplace_back(tHealthUpgrade);

	auto bHealthUpgrade = tgui::Button::create();
		bHealthUpgrade->setText("Upgrade");
		bHealthUpgrade->setPosition(bGoldUpgrade->getPosition().x, bGoldUpgrade->getPosition().y + bGoldUpgrade->getSize().y);
		bHealthUpgrade->setSize(75, 24);
		bHealthUpgrade->hide();
		bHealthUpgrade->connect("pressed", [&]() {eventManager.emit<evBuyHP>();});
		gui.add(bHealthUpgrade, "bStatHealthUpgrade");
		statWidgets.emplace_back(bHealthUpgrade);
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

void GUI::receive(const evStatsCreated &event)
{
	pStats = event.pStats;
}

void GUI::update()
{
	auto tGold = gui.get<tgui::Label>("tStatGold");
	auto tHealth = gui.get<tgui::Label>("tStatHealth");
	auto tGoldUpgrade = gui.get<tgui::Label>("tStatGoldUpgrade");
	auto tHealthUpgrade = gui.get<tgui::Label>("tStatHealthUpgrade");

	tGold->setText(std::to_string(pStats->getGold()));
	tHealth->setText(std::to_string(pStats->getHealth()));
	tGoldUpgrade->setText(std::to_string(pStats->getGold()));
	tHealthUpgrade->setText(std::to_string(pStats->getHealth()));
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