#include "MainMenu.hpp"
#include "QLearningAgent.h"
#include <SFML/Window/Event.hpp>

MainMenu::MainMenu(std::shared_ptr<Context> &context)
    : game_context(context), 
    isPlayButtonSelected(true),
    isPlayButtonPressed(false), 
    isExitButtonSelected(false),
    isExitButtonPressed(false)
{
}

MainMenu::~MainMenu()
{
}


////////////////      GET STATE ID      ////////////////
int MainMenu::GetStateID()
{
    return stateID;
}


////////////////      INITIALIZE      ////////////////
void MainMenu::Initialize()
{
    // load fonts: 
    game_context->game_assets->LoadFont(MAIN_FONT, "gfx/fonts/Futura.otf");


    // Title text definition
    gameTitle.setFont(game_context->game_assets->GetFont(MAIN_FONT));
    gameTitle.setString("SNAKE GAME");
    gameTitle.setOrigin(gameTitle.getLocalBounds().width / 2  ,  gameTitle.getLocalBounds().height / 2);
    gameTitle.setPosition(game_context->game_window->getSize().x / 2 - 80 ,  game_context->game_window->getSize().y / 2 - 150.f);
    gameTitle.setCharacterSize(60);

    // Play Button definition 
    playButton.setFont(game_context->game_assets->GetFont(MAIN_FONT));
    playButton.setString("PLAY");
    playButton.setOrigin(playButton.getLocalBounds().width / 2  ,  playButton.getLocalBounds().height / 2);
    playButton.setPosition(game_context->game_window->getSize().x / 2  ,  game_context->game_window->getSize().y / 2 - 40.f);
    playButton.setCharacterSize(40);

    // Exit Button definition 
    exitButton.setFont(game_context->game_assets->GetFont(MAIN_FONT));
    exitButton.setString("EXIT");
    exitButton.setOrigin(exitButton.getLocalBounds().width / 2  ,  exitButton.getLocalBounds().height / 2);
    exitButton.setPosition(game_context->game_window->getSize().x / 2  ,  game_context->game_window->getSize().y / 2 + 40.f);
    exitButton.setCharacterSize(40);
}

////////////////      HANDLE INPUT      ////////////////
void MainMenu::HandleInput()
{
    sf::Event event;
    while (game_context->game_window->pollEvent(event)){

        // check if window is closed
        if (event.type == sf::Event::Closed){
            game_context->game_window->close();
        }

        // check for key presses
        else if (event.type == sf::Event::KeyPressed){
            switch (event.key.code){

            case sf::Keyboard::Up:{ // MENU UP
                if (!isPlayButtonSelected){
                    isPlayButtonSelected = true;
                    isExitButtonSelected = false;
                }
                break;
            }

            case sf::Keyboard::Down:{ // MENU DOWN
                if (!isExitButtonSelected){
                    isPlayButtonSelected = false;
                    isExitButtonSelected = true;
                }
                break;
            }

            case sf::Keyboard::Return:{ // MENU SELECT OPTION 
                isPlayButtonPressed = false;
                isExitButtonPressed = false;

                if (isPlayButtonSelected){ // selecting play 
                    isPlayButtonPressed = true;
                }
                else{
                    isExitButtonPressed = true;
                }
                break;
            }

            default:{
                break;
            }

            }
        }
    }
}


////////////////      UPDATE      ////////////////
void MainMenu::Update(sf::Time deltaTime)
{
    if(isPlayButtonSelected){
        playButton.setFillColor(sf::Color::Yellow);  // highlight play button 
        exitButton.setFillColor(sf::Color(90, 90, 90, 255));
    }

    else { 
        exitButton.setFillColor(sf::Color::Yellow);  // highlight exit button 
        playButton.setFillColor(sf::Color(90, 90, 90, 255));
    }
    

    if(isPlayButtonPressed){ // play game
        game_context->game_states->AddState(std::make_unique<QLearningAgent>(game_context), true);
    }

    else if(isExitButtonPressed){ // close game
        game_context->game_window->close();
    }
    
}


////////////////      RENDER      ////////////////
void MainMenu::Render()
{
    game_context->game_window->clear(sf::Color(10, 20, 27, 255));
    game_context->game_window->draw(gameTitle);
    game_context->game_window->draw(playButton);
    game_context->game_window->draw(exitButton);
    game_context->game_window->display();
}


