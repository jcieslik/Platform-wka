#include "Resource_holder.h"

using namespace std;

sf::Texture& Resource_holder::get_texture(std::string path)
{
    auto pairFound = textures.find(path);
    if (pairFound != textures.end())
        return pairFound->second;
    else
    {
        auto& texture = textures[path];
        if (!texture.loadFromFile(path))
            throw exception("Brak pliku");  
        return texture;
    }
}
