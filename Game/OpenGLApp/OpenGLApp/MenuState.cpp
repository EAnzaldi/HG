#include "MenuState.h"
#include "PlayState.h"
#include "StateManager.h"


MenuState::MenuState(StateManager* manager) : GameState(manager), Font(NULL), CurrentSelection(0),
	CurrentGame(NULL)
{
	
}

void MenuState::ProcessInput(GLFWwindow* window, float deltatime, irrklang::ISoundEngine* engine)
{

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		SelectionDown();
	else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		SelectionUp();
	else if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
		SelectionChosen();
}


void MenuState::Draw()
{

}

void MenuState::EnterState()
{
	// Checks whether there is a current game active
	CurrentSelection = 0;
}

void MenuState::SelectionUp()
{
	if(CurrentSelection > 0)
	CurrentSelection--;

	// If there is no current game, we should skip the "Resume game" item.
	if (CurrentSelection == 1)
	{
		if (!CurrentGame || CurrentGame->IsGameOver())
			CurrentSelection--;
	}
}

void MenuState::SelectionDown()
{
	if (CurrentSelection < 1)
		CurrentSelection++;

	// If there is no current game, we should skip the "Resume game" item.
	if (CurrentSelection == 1)
	{
		if (!CurrentGame || CurrentGame->IsGameOver())
			CurrentSelection++;
	}
}

void MenuState::SelectionChosen()
{
	switch (CurrentSelection)
	{
	case 0:
		if (!CurrentGame) {
			//CurrentGame = new PlayState(Manager, Player);
		}	
		CurrentGame->Reset();
		ChangeState(CurrentGame);
		break;

	case 1:
		if (CurrentGame && !CurrentGame->IsGameOver())
			ChangeState(CurrentGame);
		break;

	case 2:
		//quit
		break;
	}
}
