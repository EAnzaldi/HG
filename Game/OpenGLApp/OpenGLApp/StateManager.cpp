#include "StateManager.h"

StateManager::StateManager() : ActiveState(nullptr)
{
}

/*

void StateManager::OnKeyDown(WPARAM wKey)
{
	if (ActiveState)
		ActiveState->OnKeyDown(wKey);
}

void StateManager::OnKeyUp(WPARAM wKey)
{
	if (ActiveState)
		ActiveState->OnKeyUp(wKey);
}

void StateManager::OnChar(WPARAM wChar)
{
	if (ActiveState)
		ActiveState->OnChar(wChar);
}

void StateManager::Update(DWORD dwCurrentTime)
{
	if (ActiveState)
		ActiveState->Update(dwCurrentTime);
}

*/

void StateManager::Draw()
{
	if (ActiveState)
		ActiveState->Draw();
}