#include "StateHandler.hpp"

Engine::StateHandler::StateHandler() : add(false), replace(false), remove(false)
{
}

Engine::StateHandler::~StateHandler()
{
}


////////////////      ADD STATE      ////////////////
void Engine::StateHandler::AddState(std::unique_ptr<State> toAdd, bool replacestate)
{
    add = true;
    game_newState = std::move(toAdd); // state to be added
    replace = replacestate; // wether the state should replace the current one
}


////////////////      POP CURRENT STATE      ////////////////
void Engine::StateHandler::PopCurrentState()
{
    remove = true; // state to be removed
}

////////////////      PROCESS STATE CHANGE      ////////////////
void Engine::StateHandler::ProcessStateChange()
{
    // check if state is to be removed and if the state stack contains a state 
    if (remove && (!game_stateStack.empty())){
        game_stateStack.pop(); // remove state

        if (!game_stateStack.empty()){ // un-pauses the game
            game_stateStack.top()->StartApplication();
        }
        remove = false; 
    }

    if (add) {
        // check if the current state is to be replaced by the new state
        if(replace && (!game_stateStack.empty())){
            game_stateStack.pop(); // remove state
            replace = false;
        }

        if (!game_stateStack.empty()){ // game is paused 
            game_stateStack.top()->PauseApplication();
        }

        game_stateStack.push(std::move(game_newState)); // add new state to the stack
        game_stateStack.top()->Initialize(); // init state before new update cycle
        add = false;
    }
}

////////////////      GET CURRENT      ////////////////
std::unique_ptr<Engine::State> &Engine::StateHandler::GetCurrent()
{
    // returns the current game state
    return game_stateStack.top();
}