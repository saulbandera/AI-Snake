#pragma once
#include <stack>
#include <memory>
#include <iostream>
#include "State.hpp"
namespace Engine
{

// [StateHandler] class used to handle the different states, with functionality 
// to add, remove, process state changes and getting the current state. 
class StateHandler
{

private:

    std::stack<std::unique_ptr<State>> game_stateStack; // stack of game states
    std::unique_ptr<State> game_newState; 

    bool add;
    bool replace;
    bool remove;

public:
    StateHandler();
    ~StateHandler();

    std::unique_ptr<State>& GetCurrent();

    void AddState(std::unique_ptr<State> toAdd, bool replace = false);
    void PopCurrentState();
    void ProcessStateChange();
};
} 
