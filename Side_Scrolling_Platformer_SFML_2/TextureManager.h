#pragma once
#include "ResourceManager.h"
#include<SFML/Graphics.hpp>
class TextureManager :
	public ResourceManager<TextureManager, sf::Texture>
{
public:
	TextureManager() : ResourceManager("Configuration/texture.cfg"){}
	~TextureManager() {}

	sf::Texture * Load(const std::string & l_path)
	{
		sf::Texture * texture = new sf::Texture();
		if (!texture->loadFromFile(Utils::GetWorkingDirection() + l_path))
		{
			delete texture;
			std::cout << "Can't open " + Utils::GetWorkingDirection() + l_path << std::endl;
			return nullptr;
		}
		return texture;
	}
};

