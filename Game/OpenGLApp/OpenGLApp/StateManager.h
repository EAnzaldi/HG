#pragma once

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
	void ProcessInput();

	void ProcessEvents();

	void MouseMoving(double xpos, double ypos);
	void MouseClick(int button, int action, int mods);

	//void Update(DWORD dwCurrentTime);
	void Render();

private: 
	// Active State of the game (intro, play, ...)
	GameState* ActiveState;
};