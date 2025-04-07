#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <glad\glad.h>
#include <GLFW/glfw3.h>
#include <irrKlang.h>

// forward declaration per evitare inclusioni circolari
class StateManager; 

// Base class for the different states of the game.
class GameState
{
public:
	GameState(StateManager* Manager);

	// The different 'events' functions
	// Child classes can implement the ones in which they are interested in
	
	virtual void ProcessInput(GLFWwindow* window, float deltatime, irrklang::ISoundEngine* engine) { }
	virtual void UpdateTime() { }
	virtual void Draw() { }

	// Functions called when the state is entered or left
	// (transition from/to another state)
	virtual void EnterState() { }
	virtual void LeaveState() { }

protected:
	// Helper function to switch to a new active state
	void ChangeState(GameState* newState);

	// The state manager
	StateManager* Manager;
};

#endif