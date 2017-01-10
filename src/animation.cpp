#include "animation.hpp"


// set protections against trying to use an animationSheet with no animation in it (current anim to 0, check if no anim before updating, etc)

animationSheet::animationSheet(sf::Texture* sheet, 
		unsigned int rowSize, 
		unsigned int totalFrames, 
		sf::Vector2i sizePerFrame,
		float speed) :
		sheet(sheet), rowSize(rowSize), totalFrames(totalFrames), iCurrentAnimation(0),
		sizePerFrame(sizePerFrame), defaultSpeed (speed), currentMode(LOOP), reversed(false)
{
}

animationSheet::~animationSheet() {};

animationSheet::animation::animation(const std::string& name, unsigned int firstFrame, unsigned int lastFrame, float speed, mode currentMode)
	: name(name), firstFrame(firstFrame), lastFrame(lastFrame), currentFrame(0), speed(speed), currentMode(currentMode)
{
	totalFrames = (lastFrame - firstFrame) + 1;
}

void animationSheet::animation::update(sf::Clock* clock)
{
	if (currentMode == LOOP)
	{
		// (time elapsed * frames per second) % total frames = index for next frame
		currentFrame = (unsigned int)fmod(clock->getElapsedTime().asSeconds() * speed, totalFrames);
	}
	// TODO SOON: handle ping pong mode 
}

void animationSheet::update()
{
	if (animationVector.size() != 0 && !currentAnimation.empty())
		animationVector[iCurrentAnimation].update(&timer);
}

sf::IntRect animationSheet::getCurrentRect() // used by sprites to get the frame on the sheet, called on every system update
{
	// e.g. frame 7 out of 10 on 3x4 grid, each frame is 64x64 and frame 1 is on top left

	unsigned int row, column, currentFrame;
	sf::Vector2i pos;
	currentFrame = animationVector[iCurrentAnimation].getCurrentFrame() + (animationVector[iCurrentAnimation].getFirstFrame() - 1);
	row = ceil(currentFrame / rowSize); // ceil(7 / 3) = 3
	column = currentFrame % rowSize; // 7 % 3 = 1
	pos.x = (column) * sizePerFrame.x; // (1 - 1) * 64 = 0
	pos.y = (row) * sizePerFrame.y; // (3 - 1) * 64 = 128

	sf::IntRect rect(pos, sf::Vector2i(sizePerFrame.x, sizePerFrame.y));
	return rect;
}

void animationSheet::addAnimation(animation& anim)
{
	animationVector.push_back(anim);
	animationMap[anim.name] = animationVector.size() - 1;

	if (currentAnimation.empty())
	{
		currentAnimation = anim.name;
		iCurrentAnimation = 0;
	}
}

void animationSheet::addAnimation(const std::string& name, unsigned int first, unsigned int last)
{

	animation newAnim (name, first, last, defaultSpeed, currentMode);
	animationVector.push_back(newAnim);
	animationMap[name] = animationVector.size() - 1;

	if (currentAnimation.empty()) // automatically set to first animation once added
	{
		currentAnimation = name;
		iCurrentAnimation = 0;
	}
}

void animationSheet::setAnimation(const std::string& name, bool _reversed)
{
	auto index = animationMap.find(name);
	if (index != animationMap.end())
	{
		animationVector[iCurrentAnimation].currentFrame = 0; // set the current anim to first frame before swapping
		currentAnimation = name;
		iCurrentAnimation = index->second;

		reversed = _reversed;
	}
	else
	{
		kk::log("setAnimation() failed, could not find \"" + name + "\"", kk::logType::error);
	}
}

void animationSheet::setAnimation(int index, bool _reversed)
{
	animationVector[iCurrentAnimation].currentFrame = 0; 
	iCurrentAnimation = index;
	currentAnimation = animationVector[iCurrentAnimation].name;

	reversed = _reversed;
}

std::string animationSheet::getCurrentAnimation()
{
	return currentAnimation;
}

unsigned int animationSheet::animation::getFirstFrame()
{
	return firstFrame;
}

unsigned int animationSheet::animation::getCurrentFrame()
{
	return currentFrame;
}

unsigned int animationSheet::getCurrentFrame()
{
	return animationVector[iCurrentAnimation].getCurrentFrame();
}

void animationSheet::setReversed(bool rev)
{
	reversed = rev;
}

void animationSheet::setReversed(sf::Sprite* box, bool _reversed)
{
	if (_reversed)
		box->setScale({ -1, 1 });
	else
		box->setScale({ 1,1 });

	reversed = _reversed;
}

bool animationSheet::getReversed()
{
	return reversed;
}

sf::Vector2i animationSheet::getSize()
{
	return sizePerFrame;
}