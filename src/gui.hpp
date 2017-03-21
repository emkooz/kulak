#pragma once
#include <vector>
#include <TGUI/TGUI.hpp>
#include <entityx/entityx.h>
#include "systems/state.hpp"

class GUI : public entityx::Receiver<GUI>
{
public:
	GUI(entityx::EntityManager& entityManager, entityx::EventManager& eventManager);
	void init(sf::RenderWindow* _window);
	void configure(entityx::EventManager& eventManager);
	void receive(const evSetState& event);
	void pollEvents(sf::Event event);
	void draw();

private:
	entityx::EntityManager& entityManager;
	entityx::EventManager& eventManager;

	template <typename T>
	tgui::Layout centerVertical(T widget, tgui::Layout width);

	template <typename T>
	tgui::Layout centerHorizontal(T widget, tgui::Layout height);
	
	tgui::Gui gui;

	// Pregame menu
	std::vector <std::shared_ptr<tgui::Widget>> statWidgets;
	
};