#include "StateManager.h"

StateManager::StateManager() : ActiveState(nullptr)
{
}

void StateManager::ProcessInput(GLFWwindow* window, float deltatime, irrklang::ISoundEngine* engine)
{
	if (ActiveState)
		ActiveState->ProcessInput(window, deltatime, engine);
}

/*



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