#pragma once
#include <memory>
#include <array>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML\System\Sleep.hpp>
#include "Game.hpp"
#include "State.hpp"
#include "Snake.hpp"
#include <iostream>

enum DIRECTIONS
{
    NONE = 0,
    UP =   -64,
    DOWN =  64,
    LEFT = -64,
    RIGHT = 64
};

enum ACTIONS { 
    up = 0,
    down,
    left, 
    right
};


// [Environment] class used to represent the environment in which the AI agent learns.
// Has functionality for: the AI to control the Snake; Updating the game and checking for
// collisions with itself or the food; Rendering the game. 

class Environment 
{
private:
    sf::Sprite grass;                
                      
    // text for UI:
    sf::Text scoreText; 
    sf::Text highScoreText;
    sf::Text scoreCountText;
    sf::Text deathCountText;
    sf::Text trainText1;

    // score related variables:
    int score;
    int highScore;
    int totalScoreCount;
    int avgDeathCounter = 0;
    int avgScoreCount = 0;

    sf::Vector2f snakeDirection; // direction in which the snake is moving (up,down,left,right)
    bool toMove; // used to prevent the snake from doing a 180 turn
    bool wrapping; // used to determine wether the snake is wrapping around the screen 

    ACTIONS lastAction; // used when setting the new snake direction so that 180 turns are not allowed
    float reward; // the reward used for reinforcement learning 

public:

    Environment(std::shared_ptr<Context> &context);
    ~Environment();

    sf::Vector2i GetFoodPosition();

    void Reset();
    void Initialize();
    void Action(ACTIONS act);
    float Update(bool paused);
    void Render();

    Snake snake;
    sf::Sprite food;
    std::shared_ptr<Context> game_context; // access to assets, states and game window
    int deathCount;
 
};