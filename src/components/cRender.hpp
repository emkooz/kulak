#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

//Render
struct cRenderable
{
	cRenderable(
		std::shared_ptr<sf::Sprite> box,
		int renderLayer = 0,
		bool render = true
	) : box(box), renderLayer(renderLayer), render(render) {}
	std::shared_ptr<sf::Sprite> box;
	int renderLayer; // change to enum later
	bool render;
};

struct cRenderableHUD
{
	cRenderableHUD(
		std::shared_ptr<sf::Sprite> box,
		int renderLayer = 0,
		bool render = true
	) : box(std::move(box)), renderLayer(renderLayer), render(render) {}
	std::shared_ptr<sf::Sprite> box;
	int renderLayer; // change to enum later
	bool render;
};

struct cRenderableText
{
	cRenderableText(
		std::shared_ptr<sf::Text> text,
		int renderLayer = 0,
		bool render = true
	) : text(text), renderLayer(renderLayer), render(render) {}
	std::shared_ptr<sf::Text> text;
	int renderLayer;
	bool render;
};

struct cRenderableTextHUD
{
	cRenderableTextHUD(
		std::shared_ptr<sf::Text> text,
		int renderLayer = 0,
		bool render = true
	) : text(text), renderLayer(renderLayer), render(render) {}
	std::shared_ptr<sf::Text> text;
	int renderLayer;
	bool render;
};

struct cRenderableRectHUD
{
	cRenderableRectHUD(
		std::shared_ptr<sf::RectangleShape> rect,
		int renderLayer = 0,
		bool render = true
	) : rect(rect), renderLayer(renderLayer), render(render) {}
	std::shared_ptr<sf::RectangleShape> rect;
	int renderLayer;
	bool render;
};

struct cRenderableMenuText
{
	cRenderableMenuText(
		std::shared_ptr<sf::Text> text,
		int renderLayer = 0,
		bool render = true
	) : text(text), renderLayer(renderLayer), render(render), selected(false) {}
	std::shared_ptr<sf::Text> text;
	int renderLayer;
	bool render;
	bool selected;
};

// this is used for HUD/GUI elements, which arent affected by world position
struct cStaticView
{
	cStaticView(int renderLayer = 0) : renderLayer(renderLayer) {}
	int renderLayer;
};

// specifically to distinguish the background. allows for render cross states
struct cBackground {};

struct cBasicRail
{
	cBasicRail(
		std::shared_ptr<sf::RectangleShape> box,
		int renderLayer = 0,
		bool render = true
	) : box(box), renderLayer(renderLayer), render(render) {}
	std::shared_ptr<sf::RectangleShape> box;
	int renderLayer;
	bool render;
	sf::Clock timeAlive;
};