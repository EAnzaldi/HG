#ifndef MENUSTATE_H
#define MENUSTATE_H

#include "GameState.h"
#include "TextObject.h"
#include "TextureObject.h"
#include "PlayState.h"

// Specialization of the GameState class for the menu state
// This displays a menu in which the player can
// - start a new game
// - continue an existing game
// - see the high-scores
// - exit the game
class MenuState : public GameState
{
public:
	MenuState(StateManager* manager);

	void ProcessInput(GLFWwindow* window, float deltatime, irrklang::ISoundEngine* engine);
	void Draw();
	void EnterState();

private:
	// The player went up or down in the menu
	void SelectionUp();
	void SelectionDown();
	// The player validated the current selection
	void SelectionChosen();

	TextObject* Font;
	// Index of the current selected menu item
	int CurrentSelection;

	// A pointer to the current active game (if any).
	PlayState* CurrentGame;

	// The background and title images
	TextureObject* BackgroundImg;
	TextureObject* TitleImg;

	// The images of the menu items (normal and selected).
	TextureObject* ItemBckgndNormal;
	TextureObject* ItemBckgndSelected;

	// The text controls of the different entries.
	TextObject* m_pNewGameText;
	TextObject* m_pResumeGameText;
	TextObject* m_pScoresText;
	TextObject* m_pExitText;
};

#endif  // _MENUSTATE_H_
