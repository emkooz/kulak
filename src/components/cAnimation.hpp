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