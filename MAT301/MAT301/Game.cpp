#include <SFML/Graphics/CircleShape.hpp>

#include "Game.hpp"
#include "MainMenu.hpp"
#include "QLearningAgent.h"

Game::Game() : game_context(std::make_shared<Context>())
{
    game_context->game_window->create(sf::VideoMode(640, 640), "Snake AI", sf::Style::Close); // create the window 
    game_context->game_states->AddState(std::make_unique<MainMenu>(game_context)); // state is set to menu screen
}

Game::~Game()
{
}


////////////////      RUN      ////////////////
void Game::Run()
{
    sf::Clock clock;
    sf::Time timeSinceLastFrame = sf::Time::Zero;

    while (game_context->game_window->isOpen()){
        timeSinceLastFrame += clock.restart();
        while (timeSinceLastFrame > dt) {
            timeSinceLastFrame -= dt;
            game_context->game_states->ProcessStateChange(); // change the game state      

            if (game_context->game_states->GetCurrent()->GetStateID() == TRAINING_1000_TIMES) {
                dt = sf::microseconds(1);
            }

            if (game_context->game_states->GetCurrent()->GetStateID() == StateID::GAME) {
                dt = sf::seconds(0.1);
            }
            game_context->game_states->GetCurrent()->HandleInput(); // handle user input
            game_context->game_states->GetCurrent()->Update(dt);    // update the game
            game_context->game_states->GetCurrent()->Render();      // render to window
        
        }   
    }
}