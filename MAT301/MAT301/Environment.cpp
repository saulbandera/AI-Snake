#include "Environment.hpp"
#include <SFML/Window/Event.hpp>
#include <stdlib.h>
#include <time.h>
#include <algorithm>



Environment::Environment(std::shared_ptr<Context>& context)
    : game_context(context),
    score(0),
    snakeDirection({ 64.f, 0.f }),
    reward(0),
    highScore(0),
    lastAction(ACTIONS::right),
    wrapping(false),
    toMove(false),
    deathCount(0),
    totalScoreCount(0)
{
    Initialize();
    srand(time(nullptr));
}

Environment::~Environment()
{
}


////////////////      RESET      ////////////////
void Environment::Reset()
{
    // used to reset the game and set the reward to negative after the 
    // snake dies

    if (score > highScore){ // check for a new highscore 
        highScore = score;
    }

    score = 0; // reset score 

    // update displayed text:
    scoreText.setString(" Score : " + std::to_string(score));
    highScoreText.setString(" Highscore : " + std::to_string(highScore));

    reward = -1; // set reward to negative since the snake has died 
    snakeDirection = sf::Vector2f(64.0f, 0); // initial direction
    lastAction = ACTIONS::right;
    snake.Reset(); // reset the snake (goes back to having size 4)
    food.setPosition(game_context->game_window->getSize().x / 2, game_context->game_window->getSize().y / 2); // start at centre
}

////////////////      INITIALIZE      ////////////////
void Environment::Initialize()
{

    // load assets for grass, food, walls and snake: 
    game_context->game_assets->LoadTexture(GRASS, "gfx/textures/grass1.png", true);
    game_context->game_assets->LoadTexture(FOOD, "gfx/textures/apple.png");
    game_context->game_assets->LoadTexture(SNAKE, "gfx/textures/snake1.png");
    game_context->game_assets->LoadTexture(SNAKEHEAD, "gfx/textures/head1.png");
    game_context->game_assets->LoadTexture(SNAKETAIL, "gfx/textures/tail.png");
    game_context->game_assets->LoadTexture(EATENFOOD, "gfx/textures/applegone.png");


    grass.setTexture(game_context->game_assets->GetTexture(GRASS)); // set the grass sprite's texture
    grass.setTextureRect(game_context->game_window->getViewport(game_context->game_window->getDefaultView()));

    food.setTexture(game_context->game_assets->GetTexture(FOOD)); // set the food texture 
    food.setPosition(game_context->game_window->getSize().x / 2, game_context->game_window->getSize().y / 2); // start at centre

    // initialize the snake:
    snake.Initialize(game_context->game_assets->GetTexture(SNAKETAIL), game_context->game_assets->GetTexture(SNAKEHEAD), game_context->game_assets->GetTexture(SNAKETAIL), game_context->game_assets->GetTexture(EATENFOOD)); // initialize the snake 


    // TEXT: 
    // define score text
    scoreText.setFont(game_context->game_assets->GetFont(MAIN_FONT));
    scoreText.setString(" Score : " + std::to_string(score));
    scoreText.setFillColor(sf::Color::Yellow);
    scoreText.setCharacterSize(30);
    // define highscore text
    highScoreText.setFont(game_context->game_assets->GetFont(MAIN_FONT));
    highScoreText.setString(" Highscore : " + std::to_string(highScore));
    highScoreText.setFillColor(sf::Color::Yellow);
    highScoreText.setCharacterSize(30);
    highScoreText.setPosition(sf::Vector2f(0, 50));
    // define death count text
    deathCountText.setFont(game_context->game_assets->GetFont(MAIN_FONT));
    deathCountText.setString(" Death count : " + std::to_string(deathCount));
    deathCountText.setFillColor(sf::Color::Yellow);
    deathCountText.setCharacterSize(30);
    deathCountText.setPosition(sf::Vector2f(0, 100));
    // define total score text
    scoreCountText.setFont(game_context->game_assets->GetFont(MAIN_FONT));
    scoreCountText.setString(" Total score : " + std::to_string(totalScoreCount));
    scoreCountText.setFillColor(sf::Color::Yellow);
    scoreCountText.setCharacterSize(30);
    scoreCountText.setPosition(sf::Vector2f(0, 150));

    // define train text
    trainText1.setFont(game_context->game_assets->GetFont(MAIN_FONT));
    trainText1.setString(" F1: Train for 1000 games");
    trainText1.setFillColor(sf::Color::Yellow);
    trainText1.setCharacterSize(30);
    trainText1.setPosition(sf::Vector2f(0, 200));
}




////////////////      ACTION      ////////////////
void Environment::Action(ACTIONS act)
{
    // Sets the direction in which the snake will move according to 
    // given action (up, down, left or right). It checks and prevents
    // the snake from making 180 degree turns. 

    sf::Vector2f newDirection = snakeDirection;

    if (act == ACTIONS::up && lastAction != ACTIONS::down && !toMove) {
        newDirection = { 0.f, DIRECTIONS::UP };
        toMove = true;
    }

    else if (act == ACTIONS::down && lastAction != ACTIONS::up && !toMove) {
        newDirection = { 0.f, DIRECTIONS::DOWN };
        toMove = true;
    } 

    else if (act == ACTIONS::left && lastAction != ACTIONS::right && !toMove) {
        newDirection = { DIRECTIONS::LEFT, 0.f  };
        toMove = true;
    }

    else if (act == ACTIONS::right && lastAction != ACTIONS::left && !toMove) {
        newDirection = { DIRECTIONS::RIGHT, 0.f };
        toMove = true;
    }

    snakeDirection = newDirection; // setting the new directin in which the snake moves 
}


////////////////      UPDATE      ////////////////
float Environment::Update(bool paused)
{

    if (!paused) { // will stop updating while the game is paused
        reward = -0.1; // a small negative reward is given for each unsuccessful move 

        // setting the last action of the snake according to it's current direction:
        if (snakeDirection.x == 0 && snakeDirection.y == DIRECTIONS::UP) {
            lastAction = ACTIONS::up;
        }

        if (snakeDirection.x == 0 && snakeDirection.y == DIRECTIONS::DOWN) {
            lastAction = ACTIONS::down;
        }

        if (snakeDirection.x == DIRECTIONS::LEFT && snakeDirection.y == 0) {
            lastAction = ACTIONS::left;
        }

        if (snakeDirection.x == DIRECTIONS::RIGHT && snakeDirection.y == 0) {
            lastAction = ACTIONS::right;
        }


        wrapping = false;

        if (snake.GetHeadPosition().x > game_context->game_window->getSize().x - 64) {
            // if snake is out of bounds on the right of the window it appears from the left
            // of the window
            snake.Move(sf::Vector2f(-float(game_context->game_window->getSize().x), 0));
            wrapping = true;
            toMove = false;
        }

        if (snake.GetHeadPosition().x < 0) {
            // if snake is out of bounds on the left of the window it appears from the right
            // of the window
            snake.Move(sf::Vector2f(float(game_context->game_window->getSize().x), 0));
            wrapping = true;
            toMove = false;
        }

        if (snake.GetHeadPosition().y < 0) {
            // if snake is out of bounds above the window it appears from the bottom
            // of the window
            snake.Move(sf::Vector2f(0, float(game_context->game_window->getSize().y)));
            wrapping = true;
            toMove = false;
        }

        if (snake.GetHeadPosition().y > game_context->game_window->getSize().y - 64) {
            // if snake is out of bounds below the window it appears from the top of
            // the window
            snake.Move(sf::Vector2f(0, -float(game_context->game_window->getSize().y)));
            wrapping = true;
            toMove = false;
        }

        //// snake is on the apple ////
        if (snake.IsOn(food)) {

            reward = 1; // reward is set to highest reward val
            totalScoreCount += 1;
            avgScoreCount += 1;
            scoreCountText.setString(" Total score : " + std::to_string(totalScoreCount));

            snake.Grow(snakeDirection); // increase snake size
            int x = 0, y = 0;

            x = (rand() % 10) * 64;
            y = (rand() % 10) * 64;
            food.setPosition(x, y); // new position of apple

            while (snake.FoodSpawnedOnBody(sf::Vector2f(x, y))) { // respawn if it was spawned on the body of the snake
                x = (rand() % 10) * 64;
                y = (rand() % 10) * 64;
                food.setPosition(x, y);
            }

            score += 1; 
            scoreText.setString(" Score : " + std::to_string(score));
        }

        //// snake moves ////
        else if (!wrapping) {
            snake.Move(snakeDirection);
            toMove = false;
        }


        //// snake collision check ////
        if (snake.IsSelfIntersecting()) {
            Reset(); // reset the game 
            deathCount += 1; // increase the number of times the snake has died 
            avgDeathCounter += 1;
            deathCountText.setString(" Death count : " + std::to_string(deathCount));
        }
    }
    return reward; // return reward obtained this loop 
}

////////////////      RENDER      ////////////////
void Environment::Render()
{
    game_context->game_window->clear();

    game_context->game_window->draw(grass);
    game_context->game_window->draw(food);
    game_context->game_window->draw(snake);
    game_context->game_window->draw(scoreText);
    game_context->game_window->draw(highScoreText);
    game_context->game_window->draw(scoreCountText);
    game_context->game_window->draw(deathCountText);
    game_context->game_window->draw(trainText1);

    game_context->game_window->display();
}


sf::Vector2i Environment::GetFoodPosition()
{
    return sf::Vector2i(food.getPosition().x, food.getPosition().y);
}


