#pragma once

#include <glm/glm.hpp>
#include <glad\glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <irrKlang.h>

#include "Camera.h"
#include "ShaderManager.h"
#include "MouseObject.h"
#include "TextureObject.h"

// forward declaration per evitare inclusioni circolari
class StateManager;

enum class GameStatus { None, Playing, Paused, GameOver, Victory, NotPlaying, End };

// Base class for the different states of the game.
class GameState
{
public:
	GameState(StateManager* manager, GLFWwindow* window, irrklang::ISoundEngine* engine);

	~GameState();

	//static GameStatus Status;//visibile a tutti gli stati

	// The different 'events' functions
	// Child classes can implement the ones in which they are interested in
	
	virtual void ProcessInput() { }
	virtual void ProcessEvents() { }

	virtual void MouseMoving(double xpos, double ypos) { }
	virtual void MouseClick(double xpos, double ypos, int button, int action, int mods) { }

	virtual void UpdateTime() { }
	virtual void Render() { }

	// Functions called when the state is entered or left
	// (transition from/to another state)
	virtual void EnterState() { }
	virtual void LeaveState() { }

	// Shaders
	Shader* pShader;
	Shader* pTextShader;
	Shader* pEnlightenedShader;
	Shader* pSpriteShader;



protected:
	// Helper function to switch to a new active state
	void ChangeState(GameState* newState);

	StateManager* Manager;// The state manager
	GLFWwindow* Window;	// The window manager
	irrklang::ISoundEngine* Engine;	// The sound manager

	MouseObject* Mouse;
	//GameObject* Mouse;

	static Camera* pCamera;
	static const glm::vec3 lightColor;
	static const glm::vec3 lightPosition;
};