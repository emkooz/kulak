#include "resource.hpp"

namespace kk
{
	static std::unordered_map <std::string, int> iTextureMap;
	static std::unordered_map <std::string, int> iFontMap;
	static std::unordered_map <std::string, int> iSoundMap;
	static std::unordered_map <std::string, int> iMusicMap;
	static std::vector <sf::Texture>     iTextureVector;
	static std::vector <sf::Font>        iFontVector;
	static std::vector <sf::SoundBuffer> iSoundVector;
	static std::vector <sf::Music>       iMusicVector;

	bool loadTexture(const std::string& name, const std::string& filepath)
	{
		iTextureVector.push_back({});

		if (!iTextureVector.back().loadFromFile(filepath))
		{
			kk::log("Failed to load texture: " + name, kk::logType::error);
			iTextureVector.pop_back();
			return false;
		}
		else
		{
			iTextureMap[name] = iTextureVector.size() - 1;
			kk::log("Loaded texture: " + name, kk::logType::normal);
			return true;
		}
	}

	sf::Texture* getTexture(const std::string& name)
	{
		auto index = iTextureMap.find(name);
		if (index != iTextureMap.end())
		{
			return &iTextureVector[index->second];
		}
		else
		{
			kk::log("Could not find texture: " + name, kk::logType::error);
			// TODO: return a default missing texture instead
			sf::Texture missing;
			missing.create(200, 200);
			return &missing;
		}
	}

	bool loadFont(const std::string& name, const std::string& filepath)
	{
		iFontVector.push_back({});

		if (!iFontVector.back().loadFromFile(filepath))
		{
			kk::log("Failed to load font: " + name, kk::logType::error);
			iFontVector.pop_back();
			return false;
		}
		else
		{
			iFontMap[name] = iFontVector.size() - 1;
			kk::log("Loaded font: " + name, kk::logType::normal);
			return true;
		}
	}

	sf::Font* getFont(const std::string& name)
	{
		auto index = iFontMap.find(name);
		if (index != iFontMap.end())
		{
			return &iFontVector[index->second];
		}
		else
		{
			kk::log("Could not find font: " + name, kk::logType::error);
			// handle this somehow
		}
	}

	bool loadSound(const std::string& name, const std::string& filepath)
	{
		iSoundVector.push_back({});

		if (!iSoundVector.back().loadFromFile(filepath))
		{
			kk::log("Failed to load sound: " + name, kk::logType::error);
			iSoundVector.pop_back();
			return false;
		}
		else
		{
			iSoundMap[name] = iSoundVector.size() - 1;
			kk::log("Loaded sound: " + name, kk::logType::normal);
			return true;
		}
	}

	sf::SoundBuffer* getSound(const std::string& name)
	{
		auto index = iSoundMap.find(name);
		if (index != iSoundMap.end())
		{
			return &iSoundVector[index->second];
		}
		else
		{
			kk::log("Could not find Sound: " + name, kk::logType::error);
			// handle this somehow
		}
	}
}