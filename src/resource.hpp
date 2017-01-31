#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <memory>
#include "log.hpp"

namespace kk
{
	bool loadTexture(const std::string& name, const std::string& filepath);
	bool loadFont(const std::string& name, const std::string& filepath);
	bool loadSound(const std::string& name, const std::string& filepath);
	bool loadMusic(const std::string& name, const std::string& filepath); // requires separate implementation
	bool loadShader(const std::string& name, const std::string& filepath, sf::Shader::Type type);
	bool loadShader(const std::string& name, const std::string& filepath1, const std::string& filepath2);

	sf::Texture*     getTexture(const std::string& name);
	sf::Font*        getFont(const std::string& name);
	sf::SoundBuffer* getSound(const std::string& name);
	sf::Music*       getMusic(const std::string& name); // not implemented
	std::shared_ptr<sf::Shader>      getShader(const std::string& name);
}