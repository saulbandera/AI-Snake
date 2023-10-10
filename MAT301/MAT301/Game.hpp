#pragma once

#include <memory>

#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>
#include "AssetHandler.hpp"
#include "StateHandler.hpp"

enum AssetID
{
    MAIN_FONT = 0,
    GRASS,
    FOOD,
    SNAKE,
    SNAKEHEAD,
    SNAKETAIL,
    EATENFOOD
};

enum StateID {
    MENU = 0,
    GAME,
    PAUSE,
    TRAINING_1000_TIMES = 1000
};

// Game context struct used to easily access the assets, states and render window of the game
struct Context
{
    // sprites used for the game
    std::unique_ptr<Engine::AssetHandler> game_assets;
    // states of the game
    std::unique_ptr<Engine::StateHandler> game_states;
    // game window
    std::unique_ptr<sf::RenderWindow> game_window;

    Context()
    {
        game_assets = std::make_unique<Engine::AssetHandler>();
        game_states = std::make_unique<Engine::StateHandler>();
        game_window = std::make_unique<sf::RenderWindow>();
    }
};


// [Game] class with the main game loop, defines a shared pointer of type Context
// used to easily gain access to the game assets, states and window. The main loop
// calls the overloaded functions of each state.
class Game
{
private:
    std::shared_ptr<Context> game_context;
    sf::Time dt = sf::seconds(1.f/60.f);

public:
    Game();
    ~Game();

    void Run();
};