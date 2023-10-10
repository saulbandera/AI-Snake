#pragma once
#include <memory>
#include <SFML/Graphics/Text.hpp>
#include "State.hpp"
#include "Game.hpp"


// [PauseGame] pause game state, displays a simple pause text
class PauseGame : public Engine::State
{
private:
    std::shared_ptr<Context> game_context; // access to assets, states and game window
    sf::Text PauseTitle; 
    int stateID = 2; 

public:
    PauseGame(std::shared_ptr<Context> &context);
    ~PauseGame();

    int GetStateID() override;

    void Initialize() override;
    void HandleInput() override;
    void Update(sf::Time deltaTime) override;
    void Render() override;
};