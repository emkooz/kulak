#include "gui.hpp"

GUI::GUI(entityx::EntityManager& entityManager, entityx::EventManager& eventManager) :
	entityManager(entityManager), eventManager(eventManager)
{
	eventManager.subscribe<evSetState>(*this);
	eventManager.subscribe<evStatsCreated>(*this);
	eventManager.subscribe<evLevelCompleted>(*this);
};

void GUI::init(sf::RenderWindow* _window)
{
	gui.setWindow(*_window);
	kk::log("Setting up GUI...");

	guiEntity = entityManager.create();
	guiEntity.assign<cSound>(); auto sound = guiEntity.component<cSound>();
	sound->spatial = false;
	std::shared_ptr<sf::Sound> clickSound(new sf::Sound()); clickSound->setBuffer(*kk::getSound("click"));
	clickSound->setRelativeToListener(true); clickSound->setPosition(0, 0, 0);
	sound->sounds.push_back(clickSound);
	sound->names.emplace_back("click");

	auto width = tgui::bindWidth(gui);
	auto height = tgui::bindHeight(gui);

	auto theme = tgui::Theme::create("red.txt");

	// TODO: update tgui to dev branch, change labels to richtext?

	// Stats widgets //
	// Play button
	tgui::Button::Ptr bPlay = theme->load("Button");
		bPlay->setSize(width * 0.1, height * 0.1);
		bPlay->setPosition(centerHorizontal(bPlay, width), height * 0.8);
		bPlay->setText("Play");
		bPlay->hide();
		bPlay->connect("pressed", [&]() {eventManager.emit<evSetState>(kk::STATE_PLAYING); eventManager.emit<evPlaySound>(guiEntity.component<cSound>(), "click"); });
		gui.add(bPlay, "bStatsPlay");
		statWidgets.emplace_back(bPlay);

	// Back button
	tgui::Button::Ptr bBack = theme->load("Button");
		bBack->setSize(26, 26);
		bBack->setPosition(5, 5);
		bBack->setText("<-");
		bBack->hide();
		bBack->connect("pressed", [&]() {eventManager.emit<evSetState>(kk::STATE_MENU); eventManager.emit<evPlaySound>(guiEntity.component<cSound>(), "click"); });
		gui.add(bBack, "bStatsBack");
		statWidgets.emplace_back(bBack);

	// Player name
	auto tName = tgui::Label::create();
		tName->setText("<name>");
		tName->setTextSize(26);
		tName->setPosition(bBack->getPosition().x + bBack->getFullSize().x + 20, 5);
		tName->hide();
		gui.add(tName, "tStatsName");
		statWidgets.emplace_back(tName);

	// Level
	auto tLevel = tgui::Label::create();
		tLevel->setText("Level 1");
		tLevel->setTextSize(36);
		tLevel->setPosition(centerHorizontal(tLevel, width), height * 0.1);
		tLevel->hide();
		gui.add(tLevel, "tStatsLevel");
		statWidgets.emplace_back(tLevel);

	// Gold text + image at top right
	auto tGold = tgui::Label::create();
		tGold->setText(std::to_string(pStats->getGold()));
		tGold->setTextSize(24);
		tGold->setPosition(width - 100, 2);
		tGold->hide();
		gui.add(tGold, "tStatsGold");
		statWidgets.emplace_back(tGold);

	auto pGold = tgui::Picture::create();
		pGold->setTexture(*kk::getTexture("coin"));
		pGold->setPosition(tGold->getPosition().x - 30, 2);
		pGold->setSize(24, 24);
		pGold->hide();
		gui.add(pGold, "pStatsGold");
		statWidgets.emplace_back(pGold);

	// Health text + image at top right
	auto tHealth = tgui::Label::create();
		tHealth->setText(std::to_string(pStats->getHealth()));
		tHealth->setTextSize(24);
		tHealth->setPosition(pGold->getPosition().x - 100, 2);
		tHealth->hide();
		gui.add(tHealth, "tStatsHealth");
		statWidgets.emplace_back(tHealth);

	auto pHealth = tgui::Picture::create();
		pHealth->setTexture(*kk::getTexture("health"));
		pHealth->setPosition(tHealth->getPosition().x - 30, 2);
		pHealth->setSize(24, 24);
		pHealth->hide();
		gui.add(pHealth, "pStatsHealth");
		statWidgets.emplace_back(pHealth);

	// Tabs to switch between player stats, weapon stats, and buying weapons
	tgui::Tab::Ptr bUpgradeTabs = theme->load("Tab");
		bUpgradeTabs->setTabHeight(26);
		bUpgradeTabs->setPosition(10, height * 0.2);
		bUpgradeTabs->add("Player Upgrades");
		bUpgradeTabs->add("Weapon Upgrades");
		bUpgradeTabs->add("Weapon Store");
		bUpgradeTabs->hide();
		bUpgradeTabs->connect("TabSelected", &GUI::tabSwitched, this);
		gui.add(bUpgradeTabs, "bStatsUpgradeTabs");
		statWidgets.emplace_back(bUpgradeTabs);

	// Max HP upgrade text + value + upgrade button
	auto tMHPText = tgui::Label::create();
		tMHPText->setText("Max HP:");
		tMHPText->setTextSize(26);
		tMHPText->setPosition(10, height * 0.3);
		tMHPText->hide();
		gui.add(tMHPText, "tStatsMHPText");
		statWidgets.emplace_back(tMHPText);
		pStatWidgets.emplace_back(tMHPText);

	auto tMHPValue = tgui::Label::create();
		tMHPValue->setText(std::to_string(pStats->getMaxHP()));
		tMHPValue->setTextSize(26);
		tMHPValue->setPosition(width * 0.3, tMHPText->getPosition().y);
		tMHPValue->hide();
		gui.add(tMHPValue, "tStatsMHPValue");
		statWidgets.emplace_back(tMHPValue);
		pStatWidgets.emplace_back(tMHPValue);

	tgui::Button::Ptr bMHPUpgrade = theme->load("Button");
		bMHPUpgrade->setText("<val> - Upgrade");
		bMHPUpgrade->setSize(100, 26);
		bMHPUpgrade->setPosition(centerHorizontal(bMHPUpgrade, width), tMHPValue->getPosition().y);
		bMHPUpgrade->hide();
		bMHPUpgrade->connect("pressed", [&]() {eventManager.emit<evBuyHP>(); eventManager.emit<evPlaySound>(guiEntity.component<cSound>(), "click"); });
		gui.add(bMHPUpgrade, "bStatsMHPUpgrade");
		statWidgets.emplace_back(bMHPUpgrade);
		pStatWidgets.emplace_back(bMHPUpgrade);

	// Max mana upgrade text + value + upgrade button
	auto tMMText = tgui::Label::create();
		tMMText->setText("Max energy:");
		tMMText->setTextSize(26);
		tMMText->setPosition(10, tMHPText->getPosition().y + tMHPText->getFullSize().y + 15);
		tMMText->hide();
		gui.add(tMMText, "tStatsMMText");
		statWidgets.emplace_back(tMMText);
		pStatWidgets.emplace_back(tMMText);

	auto tMMValue = tgui::Label::create();
		tMMValue->setText(std::to_string(pStats->getMaxMana()));
		tMMValue->setTextSize(26);
		tMMValue->setPosition(width * 0.3, tMMText->getPosition().y);
		tMMValue->hide();
		gui.add(tMMValue, "tStatsMMValue");
		statWidgets.emplace_back(tMMValue);
		pStatWidgets.emplace_back(tMMValue);

	tgui::Button::Ptr bMMUpgrade = theme->load("Button");
		bMMUpgrade->setText("<val> - Upgrade");
		bMMUpgrade->setPosition(centerHorizontal(bMHPUpgrade, width), tMMValue->getPosition().y);
		bMMUpgrade->setSize(100, 26);
		bMMUpgrade->hide();
		bMMUpgrade->connect("pressed", [&]() {eventManager.emit<evBuyMana>(); eventManager.emit<evPlaySound>(guiEntity.component<cSound>(), "click"); });
		gui.add(bMMUpgrade, "bStatsMMUpgrade");
		statWidgets.emplace_back(bMMUpgrade);
		pStatWidgets.emplace_back(bMMUpgrade);

	// MS upgrade text + value + upgrade button
	auto tMSText = tgui::Label::create();
		tMSText->setText("Movement speed:");
		tMSText->setTextSize(26);
		tMSText->setPosition(10, tMMText->getPosition().y + tMMText->getFullSize().y + 15);
		tMSText->hide();
		gui.add(tMSText, "tStatsMSText");
		statWidgets.emplace_back(tMSText);
		pStatWidgets.emplace_back(tMSText);

	auto tMSValue = tgui::Label::create();
		tMSValue->setText(std::to_string((int)pStats->getMS()));
		tMSValue->setTextSize(26);
		tMSValue->setPosition(width * 0.3, tMSText->getPosition().y);
		tMSValue->hide();
		gui.add(tMSValue, "tStatsMSValue");
		statWidgets.emplace_back(tMSValue);
		pStatWidgets.emplace_back(tMSValue);

	tgui::Button::Ptr bMSUpgrade = theme->load("Button");
		bMSUpgrade->setText("<val> - Upgrade");
		bMSUpgrade->setPosition(centerHorizontal(bMHPUpgrade, width), tMSValue->getPosition().y);
		bMSUpgrade->setSize(100, 26);
		bMSUpgrade->hide();
		bMSUpgrade->connect("pressed", [&]() {eventManager.emit<evBuyMS>(); eventManager.emit<evPlaySound>(guiEntity.component<cSound>(), "click"); });
		gui.add(bMSUpgrade, "bStatsMSUpgrade");
		statWidgets.emplace_back(bMSUpgrade);
		pStatWidgets.emplace_back(bMSUpgrade);

	// Gold gain upgrade text + value + upgrade button
	auto tGGText = tgui::Label::create();
		tGGText->setText("Gold gain:");
		tGGText->setTextSize(26);
		tGGText->setPosition(10, tMSText->getPosition().y + tMSText->getFullSize().y + 15);
		tGGText->hide();
		gui.add(tGGText, "tStatsGGText");
		statWidgets.emplace_back(tGGText);
		pStatWidgets.emplace_back(tGGText);

	auto tGGValue = tgui::Label::create();
		tGGValue->setText(std::to_string(pStats->getGoldGain()));
		tGGValue->setTextSize(26);
		tGGValue->setPosition(width * 0.3, tGGText->getPosition().y);
		tGGValue->hide();
		gui.add(tGGValue, "tStatsGGValue");
		statWidgets.emplace_back(tGGValue);
		pStatWidgets.emplace_back(tGGValue);

	tgui::Button::Ptr bGGUpgrade = theme->load("Button");
		bGGUpgrade->setText("<val> - Upgrade");
		bGGUpgrade->setPosition(centerHorizontal(bMHPUpgrade, width), tGGValue->getPosition().y);
		bGGUpgrade->setSize(100, 26);
		bGGUpgrade->hide();
		bGGUpgrade->connect("pressed", [&]() {eventManager.emit<evBuyGoldGain>(); eventManager.emit<evPlaySound>(guiEntity.component<cSound>(), "click"); });
		gui.add(bGGUpgrade, "bStatsGGUpgrade");
		statWidgets.emplace_back(bGGUpgrade);
		pStatWidgets.emplace_back(bGGUpgrade);

	// Mana/s upgrade text + value + upgrade button
	auto tMPSText = tgui::Label::create();
		tMPSText->setText("Energy/second:");
		tMPSText->setTextSize(26);
		tMPSText->setPosition(10, tGGText->getPosition().y + tGGText->getFullSize().y + 15);
		tMPSText->hide();
		gui.add(tMPSText, "tStatsMPSText");
		statWidgets.emplace_back(tMPSText);
		pStatWidgets.emplace_back(tMPSText);

	auto tMPSValue = tgui::Label::create();
		tMPSValue->setText(std::to_string((int)pStats->getManaPS()));
		tMPSValue->setTextSize(26);
		tMPSValue->setPosition(width * 0.3, tMPSText->getPosition().y);
		tMPSValue->hide();
		gui.add(tMPSValue, "tStatsMPSValue");
		statWidgets.emplace_back(tMPSValue);
		pStatWidgets.emplace_back(tMPSValue);

	tgui::Button::Ptr bMPSUpgrade = theme->load("Button");
		bMPSUpgrade->setText("<val> - Upgrade");
		bMPSUpgrade->setPosition(centerHorizontal(bMHPUpgrade, width), tMPSValue->getPosition().y);
		bMPSUpgrade->setSize(100, 26);
		bMPSUpgrade->hide();
		bMPSUpgrade->connect("pressed", [&]() {eventManager.emit<evBuyMPS>(); eventManager.emit<evPlaySound>(guiEntity.component<cSound>(), "click"); });
		gui.add(bMPSUpgrade, "bStatsMPSUpgrade");
		statWidgets.emplace_back(bMPSUpgrade);
		pStatWidgets.emplace_back(bMPSUpgrade);
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

void GUI::tabSwitched(const std::string& name)
{
	if (name == "Player Upgrades")
	{
		for (auto x : pStatWidgets)
			x->show();

		for (auto x : wStatWidgets)
			x->hide();
	}
	else if (name == "Weapon Upgrades")
	{
		for (auto x : wStatWidgets)
			x->show();

		for (auto x : pStatWidgets)
			x->hide();
	}
	else if (name == "Weapon Store")
	{
		// todo
	}

	eventManager.emit<evPlaySound>(guiEntity.component<cSound>(), "click");
}

void GUI::update()
{
		auto tGold = gui.get<tgui::Label>("tStatsGold");
		auto tHealth = gui.get<tgui::Label>("tStatsHealth");
		auto tMaxHP = gui.get<tgui::Label>("tStatsMHPValue");
		auto tMaxMana = gui.get<tgui::Label>("tStatsMMValue");
		auto tMS = gui.get<tgui::Label>("tStatsMSValue");
		auto tGoldGain = gui.get<tgui::Label>("tStatsGGValue");
		auto tMPS = gui.get<tgui::Label>("tStatsMPSValue");
		auto bMaxHP = gui.get<tgui::Button>("bStatsMHPUpgrade");
		auto bMaxMana = gui.get<tgui::Button>("bStatsMMUpgrade");
		auto bMS = gui.get<tgui::Button>("bStatsMSUpgrade");
		auto bGoldGain = gui.get<tgui::Button>("bStatsGGUpgrade");
		auto bMPS = gui.get<tgui::Button>("bStatsMPSUpgrade");

		tGold->setText(std::to_string(pStats->getGold()));
		tHealth->setText(std::to_string(pStats->getHealth()));
		tMaxHP->setText(std::to_string(pStats->getMaxHP()));
		tMaxMana->setText(std::to_string(pStats->getMaxMana()));
		tMS->setText(std::to_string((int)pStats->getMS()));
		tGoldGain->setText(std::to_string(pStats->getGoldGain()));
		tMPS->setText(std::to_string((int)pStats->getManaPS()));

		bMaxHP->setText(std::to_string(pStats->getUpgradeCost("maxHP")) + " - Upgrade");
		bMaxMana->setText(std::to_string(pStats->getUpgradeCost("maxMana")) + " - Upgrade");
		bGoldGain->setText(std::to_string(pStats->getUpgradeCost("goldGain")) + " - Upgrade");
		bMS->setText(std::to_string(pStats->getUpgradeCost("MS")) + " - Upgrade");
		bMPS->setText(std::to_string(pStats->getUpgradeCost("manaPS")) + " - Upgrade");
}

void GUI::pollEvents(sf::Event events)
{
	gui.handleEvent(events);
}

void GUI::draw()
{
	gui.draw();
}

void GUI::receive(const evStatsCreated &event)
{
	pStats = event.pStats;
}

void GUI::receive(const evLevelCompleted& event)
{
	auto text = gui.get<tgui::Label>("tStatsLevel");
	text->setText("Level " + std::to_string(event.nextLevel));
}

template <typename T>
tgui::Layout GUI::centerHorizontal(T widget, tgui::Layout width)
{
	return ((width / 2) - (widget->getFullSize().x / 2));
}

template <typename T>
tgui::Layout GUI::centerVertical(T widget, tgui::Layout height)
{
	return ((height / 2) - (widget->getFullSize().y / 2));
}