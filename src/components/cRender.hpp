#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

//Render
struct cRenderable
{
	cRenderable(
		std::unique_ptr<sf::Sprite> box,
		int renderLayer = 0,
		bool render = true
	) : box(std::move(box)), renderLayer(renderLayer), render(render) {}
	std::unique_ptr<sf::Sprite> box;
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

// this is used for HUD/GUI elements, which arent affected by world position
struct cStaticView
{
	cStaticView(int renderLayer = 0) : renderLayer(renderLayer) {}
	int renderLayer;
};

struct cBasicRail
{
	cBasicRail(
		std::unique_ptr<sf::RectangleShape> box,
		int renderLayer = 0,
		bool render = true
	) : box(std::move(box)), renderLayer(renderLayer), render(render) {}
	std::unique_ptr<sf::RectangleShape> box;
	int renderLayer;
	bool render;
	sf::Clock timeAlive;
};