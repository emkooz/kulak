#include "resource.hpp"

namespace kk
{
	static std::unordered_map <std::string, int> iTextureMap;
	static std::unordered_map <std::string, int> iFontMap;
	static std::unordered_map <std::string, int> iSoundMap;
	static std::unordered_map <std::string, int> iMusicMap;
	static std::unordered_map <std::string, int> iShaderMap;
	static std::vector <sf::Texture>     iTextureVector;
	static std::vector <sf::Font>        iFontVector;
	static std::vector <sf::SoundBuffer> iSoundVector;
	static std::vector <sf::Music>       iMusicVector;
	static std::vector <std::shared_ptr<sf::Shader>>      iShaderVector;

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
			/*sf::Texture missing;
			missing.create(200, 200);
			return &missing;*/
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
			kk::log("Could not find sound: " + name, kk::logType::error);
			// handle this somehow
		}
	}

	bool loadShader(const std::string& name, const std::string& filepath, sf::Shader::Type type)
	{
		std::shared_ptr<sf::Shader> shader (new sf::Shader);
		iShaderVector.push_back(shader);
		if (!iShaderVector.back()->loadFromFile(filepath, type))
		{
			kk::log("Failed to load shader: " + name, kk::logType::error);
			iShaderVector.pop_back();
			return false;
		}
		else
		{
			iShaderMap[name] = iShaderVector.size() - 1;
			kk::log("Loaded shader: " + name, kk::logType::normal);
			return true;
		}
	}

	bool loadShader(const std::string& name, const std::string& filepath1, const std::string& filepath2)
	{
		std::shared_ptr<sf::Shader> shader (new sf::Shader);
		iShaderVector.push_back(shader);
		if (!iShaderVector.back()->loadFromFile(filepath1, filepath2))
		{
			kk::log("Failed to load shader: " + name, kk::logType::error);
			iShaderVector.pop_back();
			return false;
		}
		else
		{
			iShaderMap[name] = iShaderVector.size() - 1;
			kk::log("Loaded shader: " + name, kk::logType::normal);
			return true;
		}
	}

	std::shared_ptr<sf::Shader> getShader(const std::string& name)
	{
		auto index = iShaderMap.find(name);
		if (index != iShaderMap.end())
		{
			return iShaderVector[index->second];
		}
		else
		{
			kk::log("Could not find shader: " + name, kk::logType::error);
		}
	}
}