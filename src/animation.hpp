#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <string>
#include <vector>
#include <unordered_map>

/*
Intended usage:
animationSheet player;
set speed, number of rows, columns, and frames
player.addAnimation("running", 8, 12); // add new animation called "running" that is from frames 8 to 12 on the sheet

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
		unsigned int columnSize, 
		unsigned int totalFrames, 
		sf::Vector2i sizePerFrame,
		float speed);
	~animationSheet();

	enum mode {LOOP, PING_PONG};

	class animation
	{
	public:
		animation (const std::string& name, unsigned int firstFrame, unsigned int lastFrame);
		void update (float deltaTime);
		unsigned int getCurrentFrame();
		std::string name;

	private:
		float speed;
		unsigned int currentFrame;
		mode currentMode;
	};

	void addAnimation (animation& anim);
	void addAnimation (const std::string& name, unsigned int first, unsigned int last);
	void setAnimation (const std::string& animation); // reset old animation on every new set animation
	void setAnimation (int animation);
	void setMode (mode _mode);
	void setSpeed (float speed);
	void update (float deltaTime);

	std::string getCurrentAnimation();
	mode getMode();
	float getDefaultSpeed();
	sf::IntRect getCurrentRect();
	unsigned int getCurrentFrame();

private:
	sf::Texture* sheet;
	std::string name;
	sf::Vector2i sizePerFrame; // might possibly want to move this into the animation class, allow different animation to be dynamic	
	unsigned int rowSize, columnSize, totalFrames;
	std::unordered_map<std::string, unsigned int> animationMap;
	std::vector<animation> animationVector;
	mode currentMode;
	float defaultSpeed; // speed is in frames per second
	std::string currentAnimation; // used for external purposes, being able to easily read the current animation
	unsigned int iCurrentAnimation; // used internally to directly acces the vector instead of doing a map lookup with string
};