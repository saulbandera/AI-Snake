#pragma once
#include <SFML/System/Time.hpp>

namespace Engine
{
// [State] class used to define the virtual functions the different states have

class State
{
public:
    State(){};
    virtual ~State() {};

    // methods required for the different states:
    virtual int GetStateID() = 0;
    virtual void Initialize() = 0;
    virtual void HandleInput() = 0;
    virtual void Update(sf::Time deltaTime) = 0;
    virtual void Render() = 0;
    virtual void PauseApplication() {};
    virtual void StartApplication() {};
      
    };
    
}
