#pragma once
#include <entityx/entityx.h>
#include <SFML/Graphics.hpp>
#include "components/events.hpp"
#include "components/cRender.hpp"
#include "player_stats.hpp"

class hudSystem : public entityx::System<hudSystem>, public entityx::Receiver<hudSystem>
{
public:
	hudSystem(entityx::EntityManager& entityManager, entityx::EventManager& eventManager, sf::RenderWindow* window);
	void configure(entityx::EventManager& eventManager);
	void update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt);
	void receive(const evStatsCreated &event);
	void receive(const evSwitchWeapon &event);

private:
	entityx::EntityManager& entityManager;
	entityx::EventManager& eventManager;
	entityx::Entity pEntity;
	sf::RenderWindow* window;
	//sf::View cameraView;
	stats* pStats;

	// HUD is constructed manually, since it never changes. update() handles updating strings and textures
	entityx::Entity hpTexture;
	entityx::Entity hpText;
	entityx::Entity goldTexture;
	entityx::Entity goldText;
	entityx::Entity rail;
	entityx::Entity melee;
	entityx::Entity selectedWeapon;
};