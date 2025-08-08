#ifndef MENUSTATE_H
#define MENUSTATE_H

#include "GameState.h"
#include "PlayState.h"
#include "TextObject.h"
#include "TextureObject.h"
#include "FlatMesh.h"

// Specialization of the GameState class for the menu state
// This displays a menu in which the player can
// - start a new game
// - continue an existing game
// - see the high-scores
// - exit the game
class MenuState : public GameState
{
public:
	~MenuState();

	void ProcessInput();
	void Render();
	void EnterState();

	void MouseMoving(double xpos, double ypos);
	void MouseClick(int button, int action, int mods);

	// Returns the single instance (-> singleton)
	static MenuState* GetInstance(StateManager* manager, GLFWwindow* window, irrklang::ISoundEngine* engine);

protected:
	MenuState(StateManager* manager, GLFWwindow* window, irrklang::ISoundEngine* engine);

private:
	// The player went up or down in the menu
	void SelectionUp();
	void SelectionDown();
	// The player validated the current selection
	void SelectionChosen();

	// Index of the current selected menu item
	int CurrentSelection;

	// A pointer to the current active game (if any).
	PlayState* CurrentGame;

	Shader* pShader;
	Shader* pTextShader;
	Shader* pSpriteShader;

	// The background image
	FlatMesh* pBackground;
	GameObject* pBackgroundObj;

	// The menu images
	FlatMesh* pMenu[3];
	GameObject* pMenuObj[3];
	FlatMesh* pMenuSel[3];
	GameObject* pMenuSelObj[3];
	FlatMesh* pMenuNoGame;
	GameObject* pMenuNoGameObj;

	FlatMesh* pTest;
	GameObject* pTestObj;

	TextureObject* pTitleTex;
	Model* pTitleModel;
	GameObject* pTitleObj;

	FT_Library ft;

	// The text controls of the different entries.
	TextObject* pTextNormal;
	TextObject* pTitle;



	Camera* pCamera;
};

#endif  // _MENUSTATE_H_
