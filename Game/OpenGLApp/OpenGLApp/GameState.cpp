#include "GameState.h"
#include "StateManager.h"

GameState::GameState(StateManager* manager)
	: Manager(manager)
{
}

void GameState::ChangeState(GameState* newState)
{
	Manager->ChangeState(newState);
}