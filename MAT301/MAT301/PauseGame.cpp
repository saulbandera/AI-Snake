#include "PauseGame.hpp"

#include <SFML/Window/Event.hpp>

PauseGame::PauseGame(std::shared_ptr<Context> &context)
    : game_context(context)
{
}

PauseGame::~PauseGame()
{
}

////////////////      GET STATE ID      ////////////////
int PauseGame::GetStateID()
{
    return stateID;
}

////////////////      INITIALIZE      ////////////////
void PauseGame::Initialize()
{

    // Paused text def: 
    PauseTitle.setFont(game_context->game_assets->GetFont(MAIN_FONT));
    PauseTitle.setString("Paused");
    PauseTitle.setOrigin(PauseTitle.getLocalBounds().width / 2   ,   PauseTitle.getLocalBounds().height / 2);
    PauseTitle.setPosition(game_context->game_window->getSize().x / 2  ,   game_context->game_window->getSize().y / 2);
}


////////////////      HANDLE INPUT      ////////////////
void PauseGame::HandleInput()
{

    sf::Event event;
    while (game_context->game_window->pollEvent(event)){

        if (event.type == sf::Event::Closed){
            game_context->game_window->close();
        }

        else if (event.type == sf::Event::KeyPressed){

            switch (event.key.code){

            case sf::Keyboard::Escape:{
                game_context->game_states->PopCurrentState(); // remove pause state (unpause)
                break;
            }

            default:
                break;
            
            }
        }
    }
}


////////////////      UPDATE      ////////////////
void PauseGame::Update(sf::Time deltaTime)
{

}


////////////////      RENDER      ////////////////
void PauseGame::Render()
{
    game_context->game_window->draw(PauseTitle);
    game_context->game_window->display();
}
