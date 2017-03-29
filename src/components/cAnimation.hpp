#pragma once
#include "animation.hpp"
#include <SFML/Graphics/Texture.hpp>

struct cAnimation
{
	cAnimation(sf::Texture* sheet,
		unsigned int rowSize,
		unsigned int totalFrames,
		sf::Vector2i sizePerFrame,
		float speed) : animations(sheet, rowSize, totalFrames, sizePerFrame, speed) {};

	animationSheet animations;
};


// the index of the animation for the main entity MUST equal the layer it is a part of
struct cAnimationLayered
{
	cAnimationLayered() : state(""), entityLayer(0) {};
	std::string state;
	int entityLayer; // this is the layer that the main entity has. for the player its the body
	std::vector<animationSheet> animations;
	std::vector<cRenderable> otherLayers;
};