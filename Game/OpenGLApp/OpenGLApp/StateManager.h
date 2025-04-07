#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include "GameState.h"

class StateManager
{
public:

	StateManager();

	// Switches to another active state.
	void ChangeState(GameState* newState)
	{
		if (ActiveState)
			ActiveState->LeaveState();
		ActiveState = newState;
		ActiveState->EnterState();
	}

	// Returns the current active state
	GameState* GetActiveState() { return ActiveState; }

	// 'Events' function are redirected to the active state
	void ProcessInput(GLFWwindow* window, float deltatime, irrklang::ISoundEngine* engine);
	//void Update(DWORD dwCurrentTime);
	void Draw();

private: 
	// Active State of the game (intro, play, ...)
	GameState* ActiveState;
};

#endif