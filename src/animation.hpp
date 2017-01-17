#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <string>
#include <vector>
#include <cmath>
#include <unordered_map>
#include "log.hpp"
#include "resource.hpp"
#include "components/cPlayer.hpp"

/*
Intended usage:
animationSheet player;
set speed, number of rows, columns, and frames
frames start at 1, not 0!
player.addAnimation("running", 5, 12); // add new animation called "running" that is from frames 5 to 12 on the sheet

...animation system will update each running animation on an entity...

drawing is something like:
sprite.setrect(animation.getrect());
draw(sprite);
*/

class animationSheet
{
public:
	animationSheet(sf::Texture* sheet, 
		unsigned int rowSize, 
		unsigned int totalFrames, 
		sf::Vector2i sizePerFrame,
		float speed);
	~animationSheet();

	enum mode {LOOP, PING_PONG};

	class animation
	{
	public:
		animation (const std::string& name, unsigned int firstFrame, unsigned int lastFrame, float speed, mode currentMode);
		void update (sf::Clock* clock);
		unsigned int getCurrentFrame();
		unsigned int getFirstFrame();
		std::string name;
		unsigned int currentFrame;

	private:
		float speed;
		mode currentMode;
		unsigned int firstFrame, lastFrame, totalFrames;
	};

	void addAnimation (animation& anim);
	void addAnimation (const std::string& name, unsigned int first, unsigned int last);
	void setAnimation (const std::string& name, bool reversed); // reset old animation on every new set animation
	void setAnimation (int animation, bool reversed);
	void setMode (mode _mode);
	void setSpeed (float speed);
	void setReversed(bool reversed);
	void setReversed(sf::Sprite* box, bool reversed);
	bool getReversed();
	sf::Vector2f getSize();
	void update ();

	std::string getCurrentAnimation();
	mode getMode();
	float getDefaultSpeed();
	sf::IntRect getCurrentRect();
	unsigned int getCurrentFrame();

private:
	sf::Texture* sheet;
	std::string name;
	sf::Vector2i sizePerFrame; // might possibly want to move this into the animation class, allow different animation to be dynamic	
	unsigned int rowSize, totalFrames;
	std::unordered_map<std::string, unsigned int> animationMap;
	std::vector<animation> animationVector;
	mode currentMode;
	float defaultSpeed; // speed is in frames per second
	std::string currentAnimation; // used for external purposes, being able to easily read the current animation
	unsigned int iCurrentAnimation; // used internally to directly acces the vector instead of doing a map lookup with string
	sf::Clock timer; // internal clock used to update animations
	bool reversed;
};