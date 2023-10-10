#pragma once
#include <iostream>
#include <map>
#include <string>
#include "Environment.hpp"
#include "Game.hpp"


// [QLearningAgent] class used to represent the AI agent, has a Q-Table in
// form of an std::map with the state and actions, has the main functionality
// of running the environment and updating the Q-table so that the AI learns. 
class QLearningAgent : public Engine::State
{
private:
	// array used to hold the probablity that each of the actions 
	// that the snake can perform will be successful. 
	struct Values { 
		float actions[4];
	};

	enum STATES {
		DEFAULT = 1,
		TRAINING_1000_TIMES = 1000
	};

public:
	QLearningAgent(std::shared_ptr<Context>& context);
	~QLearningAgent();

	std::string GetState();
	ACTIONS GetBestAction(std::string state); 
	int GetStateID() override;

	void Initialize() override;
	void HandleInput() override;
	void Render() override;
	void Update(sf::Time deltaTime) override;
	void PauseApplication() override;
	void StartApplication() override;

	void UpdateQTable(std::string state1, std::string state2, float rewardObtained, ACTIONS actionPerformed);

	Environment* environment; // the snake game 
	std::shared_ptr<Context> game_context; // access to assets, states and game window

	
private:

	ACTIONS availableActions[4] = { up, down, left, right };
	std::map<std::string, Values> qTable; // map used as the q table

	bool paused;
	bool toPause;
	bool training; 
	int trainingDeathCount;
	// Q(s, act) += LR * (rew + DF*max(Q(s',*)) - Q(s,act))

	// learning rate close to 0 will make learning slow
	// values closer to 1 will override old information
	// in favor of new information:
	float learningRate = 0.1; 
	float discountFactor = 0.9; // determines importance of future rewards 
	float randomize = 50; // the amount of random actions taken (5% chance)
	int stateID = 1;
};

