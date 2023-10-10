#include "AssetHandler.hpp"

Engine::AssetHandler::AssetHandler()
{
}

Engine::AssetHandler::~AssetHandler()
{
}

////////////////      GET TEXTURE      ////////////////
const sf::Texture& Engine::AssetHandler::GetTexture(int id) const
{
    return *(game_textures.at(id).get());
}


////////////////      GET FONT      ////////////////
const sf::Font& Engine::AssetHandler::GetFont(int id) const
{
    return *(game_fonts.at(id).get());
}




////////////////      LOAD TEXTURE      ////////////////
void Engine::AssetHandler::LoadTexture(int texId, const std::string &filePath, bool repeated)
{
    auto texture = std::make_unique<sf::Texture>();

    if(texture->loadFromFile(filePath)){ // check if loading texture was successful 
        texture->setRepeated(repeated);
        game_textures[texId] = std::move(texture); // add texture to the map of textures
    }
}


////////////////      LOAD FONT      ////////////////
void Engine::AssetHandler::LoadFont(int fontId, const std::string &filePath)
{
    auto font = std::make_unique<sf::Font>();

    if(font->loadFromFile(filePath)){ // check if loading font was successful 
        game_fonts[fontId] = std::move(font); // add font to the map of fonts
    }
}


