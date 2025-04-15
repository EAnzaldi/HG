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
	GameState(StateManager* manager, GLFWwindow* window, irrklang::ISoundEngine* engine);

	~GameState();

	// The different 'events' functions
	// Child classes can implement the ones in which they are interested in
	
	virtual void ProcessInput() { }
	virtual void UpdateTime() { }
	virtual void Render() { }

	// Functions called when the state is entered or left
	// (transition from/to another state)
	virtual void EnterState() { }
	virtual void LeaveState() { }

protected:
	// Helper function to switch to a new active state
	void ChangeState(GameState* newState);

	// The state manager
	StateManager* Manager;
	GLFWwindow* Window;
	irrklang::ISoundEngine* Engine;

};

#endif