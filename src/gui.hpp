#pragma once
#include <vector>
#include <TGUI/TGUI.hpp>
#include <entityx/entityx.h>
#include "systems/state.hpp"
#include "player_stats.hpp"
#include "resource.hpp"

class GUI : public entityx::Receiver<GUI>
{
public:
	GUI(entityx::EntityManager& entityManager, entityx::EventManager& eventManager);
	void init(sf::RenderWindow* _window);
	void receive(const evSetState& event);
	void receive(const evStatsCreated& event);
	void receive(const evLevelCompleted& event);
	void pollEvents(sf::Event event);
	void draw();
	void update();

private:
	entityx::EntityManager& entityManager;
	entityx::EventManager& eventManager;
	stats* pStats;

	template <typename T>
	tgui::Layout centerVertical(T widget, tgui::Layout width);

	template <typename T>
	tgui::Layout centerHorizontal(T widget, tgui::Layout height);
	
	tgui::Gui gui;

	// Pregame menu
	std::vector <std::shared_ptr<tgui::Widget>> statWidgets;
	// "update" vector?
};