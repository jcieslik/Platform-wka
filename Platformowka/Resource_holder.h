#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>

class Resource_holder
{
	Resource_holder() : textures() {};
	Resource_holder(const Resource_holder&);
	std::map<std::string, sf::Texture> textures;
public:
	static Resource_holder& get_resources()
	{
		static Resource_holder resources;
		return resources;
	}
	sf::Texture& get_texture(std::string path);
};

