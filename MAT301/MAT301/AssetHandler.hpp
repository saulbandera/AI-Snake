#pragma once
#include <map>
#include <memory>
#include <string>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>

namespace Engine
{
// [AssetHandler] class used to load in game textures and fonts 
// and functionality to get them by id.
class AssetHandler
{
private:
    std::map<int, std::unique_ptr<sf::Texture>> game_textures;
    std::map<int, std::unique_ptr<sf::Font>> game_fonts;

public:
    AssetHandler();
    ~AssetHandler();

    const sf::Texture& GetTexture(int id) const;
    const sf::Font& GetFont(int id) const;

    void LoadTexture(int texId, const std::string &filePath, bool repeated = false);
    void LoadFont(int fontId, const std::string &filePath);


};
} 
