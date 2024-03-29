#pragma once
#include <entityx/entityx.h>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "resource.hpp"
#include "log.hpp"
#include "background.hpp"
#include "gui.hpp"
#include "systems/state.hpp"
#include "systems/input.hpp"
#include "systems/player_movement.hpp"
#include "systems/animation_system.hpp"
#include "systems/enemy_spawn.hpp"
#include "systems/enemy_ai.hpp"
#include "systems/weapons.hpp"
#include "systems/player_weapons.hpp"
#include "systems/camera.hpp"
#include "systems/player_resources.hpp"
#include "systems/hud_system.hpp"
#include "systems/state.hpp"
#include "systems/menu.hpp"
#include "systems/enemy_weapons.hpp"
#include "systems/level_system.hpp"
#include "systems/sound_system.hpp"
#include "components/cPlayer.hpp"
#include "components/cAnimation.hpp"
#include "components/cWeapon.hpp"
#include "components/cRender.hpp"

class World : public entityx::EntityX, public entityx::Receiver<World>
{
public:
	World(sf::RenderWindow* _window);
	void configure(entityx::EventManager &event_manager);
	void update(sf::Time deltaTime);
	void receive(const evPause& pause);
	void receive(const evResume& resume);
	void receive(const evQuit& quit);
	void createEntities(entityx::EventManager& event_manager);
	void createSystems();
	void pollGui(sf::Event ev);
	void renderGui();

	entityx::Entity ePlayer; // temporary
private:

	sf::RenderWindow* window;
	background bg;
	GUI gui;
};