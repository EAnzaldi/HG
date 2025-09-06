#include "StateManager.h"

StateManager::StateManager() : ActiveState(nullptr)
{

}
void StateManager::ProcessInput()
{
	if (ActiveState)
		ActiveState->ProcessInput();
}
void StateManager::ProcessEvents()
{
	if (ActiveState)
		ActiveState->ProcessEvents();
}
void StateManager::MouseMoving(double xpos, double ypos) {
	if (ActiveState)
		ActiveState->MouseMoving(xpos, ypos);
}
void StateManager::MouseClick(int button, int action, int mods) {
	if (ActiveState)
		ActiveState->MouseClick(button, action, mods);
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