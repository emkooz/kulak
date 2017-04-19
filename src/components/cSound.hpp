#pragma once
#include <SFML/Audio.hpp>
#include <memory>
#include <vector>
#include <string>


struct cSound
{
	std::vector<std::shared_ptr<sf::Sound>> sounds;
	std::vector<std::string> names;
	bool spatial;
};