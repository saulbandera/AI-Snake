#pragma once
#include <SFML/Graphics/Text.hpp>
#include <memory>
#include "State.hpp"
#include "Game.hpp"

class MainMenu : public Engine::State
{
private:

    std::shared_ptr<Context> game_context; // used to get access to assets, states and render window
    
    sf::Text gameTitle;
    sf::Text playButton;
    sf::Text exitButton;

    bool isPlayButtonSelected;
    bool isPlayButtonPressed;
    bool isExitButtonSelected;
    bool isExitButtonPressed;

    int stateID = 0;
public:
    MainMenu(std::shared_ptr<Context> &context);
    ~MainMenu();

    int GetStateID() override;

    void Initialize() override;
    void HandleInput() override;
    void Update(sf::Time deltaTime) override;
    void Render() override;
};