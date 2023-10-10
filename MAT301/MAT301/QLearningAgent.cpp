#include "QLearningAgent.h"
#include <SFML/Window/Event.hpp>
#include "PauseGame.hpp"

QLearningAgent::QLearningAgent(std::shared_ptr<Context>& context):
    game_context(context),
    paused(false),
    toPause(false),
    training(false),
    trainingDeathCount(0)
{ 
    environment = new Environment(game_context); // initialize the snake game 
    srand(time(nullptr));
}

QLearningAgent::~QLearningAgent()
{
}



////////////////      GET STATE      ////////////////
std::string QLearningAgent::GetState()
{
    // the state is represented by a string containing the relative position
    // of the food to the head of the snake followed by the relative position
    // of the tail of the snake to it's head. Example: 64,256,512,576

    sf::Vector2f playerHeadPos = environment->snake.ACTUALHEAD->getPosition(); // get head position
    sf::Vector2f playerTailPos = environment->snake.tail->getPosition(); // get tail position
    sf::Vector2f foodPos = environment->food.getPosition(); // get food position
    sf::Vector2i relativeFoodPosToHead = { 0,0 };
    sf::Vector2i relativeTailPosToHead = { 0,0 };
    std::string stateName = "";


    // calculating the relative X position of the food to the head: 
    relativeFoodPosToHead.x = foodPos.x - playerHeadPos.x;
    while (relativeFoodPosToHead.x < 0) { // taking screen wrapping into account 
        relativeFoodPosToHead.x += 10*64; 
    }
    while (relativeFoodPosToHead.x > 9*64) {
        relativeFoodPosToHead.x -= 10*64;
    }

    // calculating the relative Y position of the food to the head: 
    relativeFoodPosToHead.y = foodPos.y - playerHeadPos.y;
    while (relativeFoodPosToHead.y < 0) {
        relativeFoodPosToHead.y += 10*64;
    }
    while (relativeFoodPosToHead.y > 9*64) {
        relativeFoodPosToHead.y -=10*64;
    }

    // setting the first part of the string representing the state: 
    stateName = std::to_string(relativeFoodPosToHead.x) + "," + std::to_string(relativeFoodPosToHead.y);



    // calculating the relative X position of the tail to the head: 
    relativeTailPosToHead.x = playerTailPos.x - playerHeadPos.x;
    while (relativeTailPosToHead.x < 0) { // taking screen wrapping into account
        relativeTailPosToHead.x += 10*64;
    }
    while (relativeTailPosToHead.x > 9*64) {
        relativeTailPosToHead.x -= 10*64;
    }

    // calculating the relative Y position of the tail to the head: 
    relativeTailPosToHead.y = playerTailPos.y - playerHeadPos.y;
    while (relativeTailPosToHead.y < 0) {
        relativeTailPosToHead.y += 10*64;
    }
    while (relativeTailPosToHead.y > 9*64) {
        relativeTailPosToHead.y -= 10*64;
    }

    // setting the second part of the string representing the state: 
    stateName += ",";
    stateName += std::to_string(relativeTailPosToHead.x) + "," + std::to_string(relativeTailPosToHead.y);

    return stateName;
}



////////////////      GET BEST ACTION      ////////////////
ACTIONS QLearningAgent::GetBestAction(std::string state)
{

    // checks the q table for the current state and returns the action with
    // the highest probability of success, there is a small chance a random
    // action is returned to promote exploration. If the highest probablity
    // is 0 then that action is taken, or, if there are multiple actions with
    // probablity 0 then a random selection is made out of them. 



    Values acts = qTable[state]; // getting the probablities of a move being successful for the given state

    if (rand() % 100 < 5) { // a small chance a random action is selected
        return availableActions[rand() % 4];
    }

    float maxVal = acts.actions[0]; // setting inital max value
    ACTIONS chosenAction = availableActions[0]; 

    // vector containing actions that havent been performed yet
    // (probablity of succes being 0 since it is unknown):
    std::vector<ACTIONS> actsZero; 

    for (int i = 0; i < 4; i++) {

        if (acts.actions[i] == 0) {
            actsZero.push_back(availableActions[i]);
        }

        if (acts.actions[i] > maxVal) { // new maximum value is set
            maxVal = acts.actions[i];
            chosenAction = availableActions[i];
        }
    }

    if (maxVal == 0) { // make a random selection out of the unperformed actions
        chosenAction =  actsZero[rand() % (actsZero.size())];
    }

    return chosenAction;
}



////////////////      UPDATE Q TABLE      ////////////////
void QLearningAgent::UpdateQTable(std::string state0, std::string state1, float rewardObtained, ACTIONS actionPerformed)
{

    // the probablity of an action being successful is updated after the action is
    // performed and knowing the reward obtained from it. The value is updated according
    // to the reward obtained from the current state and the maximum probablity of success
    // of the next state (weighted by the learning rate and discount rate accordingly). 
    
    Values q0 = qTable[state0]; // current state
    Values q1 = qTable[state1]; // next state

    float maxValq1 = q1.actions[0]; // getting the maximum reward that can be obtained from next state:
    for (int i = 0; i < 4; i++) {
        if (q1.actions[i] > maxValq1) {
            maxValq1 = q1.actions[i];
        }
    }
    
    float newValueForAction = rewardObtained + discountFactor * maxValq1 - q0.actions[actionPerformed];

    // value for action is updated: 
    qTable[state0].actions[actionPerformed] = q0.actions[actionPerformed] + learningRate * newValueForAction;

}


////////////////      INITIALIZE      ////////////////
void QLearningAgent::Initialize()
{
    environment->game_context->game_window->setActive(true);
}


////////////////      GET STATE ID      ////////////////
int QLearningAgent::GetStateID()
{
    return stateID;
}


////////////////      HANDLE INPUT      ////////////////

void QLearningAgent::HandleInput()
{
    sf::Event event;
    while (game_context->game_window->pollEvent(event)) {
        // check if window is closed
        if (event.type == sf::Event::Closed) {
            game_context->game_window->close();
        }

        // check for key presses
        else if (event.type == sf::Event::KeyPressed) {

            switch (event.key.code) {



            case sf::Keyboard::F1:
                if(stateID == DEFAULT)
                stateID = TRAINING_1000_TIMES;
                trainingDeathCount = environment->deathCount;                 
                break;

            case sf::Keyboard::Escape:
                // add pause game state to the state stack (pausing the game). 
                game_context->game_states->AddState(std::make_unique<PauseGame>(game_context));
                toPause = true;
                break;

            default:
                break;
            }
        }
    }
}


////////////////      RENDER      ////////////////
void QLearningAgent::Render()
{


    if (stateID == DEFAULT) {
        environment->Render();
    }

    else {
        int dif = environment->deathCount - trainingDeathCount;

        if (stateID == TRAINING_1000_TIMES && dif > 1000) {
            stateID = DEFAULT;
        }
    }


 


    
}

////////////////      UPDATE      ////////////////
void QLearningAgent::Update(sf::Time deltaTime)
{
    // main algorithm: The current state is obtained. The best action for that
    // state is obtained and the agent performs the action. The environment is
    // updated and a reward is returned. Then, using the reward and the new state
    // the Q-table is updated. 

    if (!paused) {

        std::string currentState = GetState(); 

        ACTIONS actionToPerform = GetBestAction(currentState); 

        environment->Action(actionToPerform);

        float instantReward = 0;
        instantReward = environment->Update(toPause);

        std::string nextState = GetState(); // get the state after action (next state)

        UpdateQTable(currentState, nextState, instantReward, actionToPerform);
    }
}


////////////////      PAUSE APPLICATION      ////////////////
void QLearningAgent::PauseApplication()
{
    paused = true;
}


////////////////      START APPLICATION      ////////////////
void QLearningAgent::StartApplication()
{
    toPause = false;
    paused = false;
}