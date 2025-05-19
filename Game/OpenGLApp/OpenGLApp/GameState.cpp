#include "GameState.h"
#include "StateManager.h"
#include <iostream>


GameState::GameState(StateManager* manager, GLFWwindow* window, irrklang::ISoundEngine* engine) : Manager(manager), Window(window), Engine(engine)
{

}

GameState::~GameState() {

}

void GameState::ChangeState(GameState* newState)
{
	Engine->stopAllSounds();
	Manager->ChangeState(newState);
}