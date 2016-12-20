#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <unordered_map>
#include "log.hpp"

namespace kk
{
	bool loadTexture(const std::string& name, const std::string& filepath);
	bool loadFont(const std::string& name, const std::string& filepath);
	bool loadSound(const std::string& name, const std::string& filepath);
	bool loadMusic(const std::string& name, const std::string& filepath); // requires separate implementation

	sf::Texture*     getTexture(const std::string& name);
	sf::Font*        getFont(const std::string& name);
	sf::SoundBuffer* getSound(const std::string& name);
	sf::Music*       getMusic(const std::string& name); // not implemented
}