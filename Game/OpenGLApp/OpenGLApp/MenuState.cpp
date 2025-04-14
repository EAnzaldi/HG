#include "MenuState.h"
#include "PlayState.h"
#include "StateManager.h"


MenuState::MenuState(StateManager* manager, GLFWwindow* window, irrklang::ISoundEngine* engine)
	: GameState(manager, window, engine), Font(NULL), CurrentSelection(0), CurrentGame(NULL)
{
	
}

MenuState::~MenuState() {
	//Distrugge FreeType
	FT_Done_FreeType(ft);
}

void MenuState::ProcessInput()
{

	if (glfwGetKey(Window, GLFW_KEY_DOWN) == GLFW_PRESS)
		SelectionDown();
	else if (glfwGetKey(Window, GLFW_KEY_UP) == GLFW_PRESS)
		SelectionUp();
	else if (glfwGetKey(Window, GLFW_KEY_ENTER) == GLFW_PRESS)
		SelectionChosen();
}


void MenuState::Render()
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
