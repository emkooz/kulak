#include "animation.hpp"


// set protections against trying to use an animationSheet with no animation in it (current anim to 0, check if no anim before updating, etc)

animationSheet::animationSheet(sf::Texture* sheet, 
		unsigned int rowSize, 
		unsigned int columnSize, 
		unsigned int totalFrames, 
		sf::Vector2i sizePerFrame,
		float speed) :
		sheet(sheet), rowSize(rowSize), columnSize(columnSize),
		totalFrames(totalFrames), sizePerFrame(sizePerFrame), defaultSpeed (speed), currentMode(LOOP)
{

}

animationSheet::~animationSheet() {};

void animationSheet::addAnimation(animation& anim)
{
	animationVector.push_back(anim);
	animationMap[anim.name] = animationVector.size() - 1;
}

void animationSheet::addAnimation(const std::string& name, unsigned int first, unsigned int last)
{
	animation newAnim (name, first, last);
	animationVector.push_back(newAnim);
	animationMap[name] = animationVector.size() - 1;
}

void animationSheet::setAnimation(const std::string& name)
{
	auto index = animationMap.find(name);
	if (index != animationMap.end())
	{

	}
	else
	{
		kk::log("setAnimation() failed, could not find \"" + name "\"", kk::logType::error);
	}
}