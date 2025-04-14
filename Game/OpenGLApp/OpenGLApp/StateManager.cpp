#include "StateManager.h"

StateManager::StateManager() : ActiveState(nullptr)
{
}

void StateManager::ProcessInput()
{
	if (ActiveState)
		ActiveState->ProcessInput();
}

/*



void StateManager::Update(DWORD dwCurrentTime)
{
	if (ActiveState)
		ActiveState->Update(dwCurrentTime);
}

*/

void StateManager::Render()
{
	if (ActiveState)
		ActiveState->Render();
}