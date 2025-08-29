#pragma once

#include "StateManager.h"
#include "GameState.h"
#include "PlayState.h"
#include "TextObject.h"
#include "TextureObject.h"
#include "FlatMesh.h"
#include "GameObject.h"

class PlayState; //forward declaration per evitare loop di inclusioni

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

	void MouseMoving(double xpos, double ypos) override;
	void MouseClick(int button, int action, int mods) override;

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

	// 
	// of the current selected menu item
	int CurrentSelection;

	//FlatMesh* fmesh; //unica mesh quadrata per sprite

	// A pointer to the current active game (if any).
	PlayState* CurrentGame;

	// The background image
	TextureObject* pBackground;
	GameObject* pBackgroundObj;

	// The menu images
	TextureObject* pMenu[3];
	TextureObject* pMenuSel[3];
	TextureObject* pMenuNoGame;
	TextureObject* pMenuMod[2];

	GameObject* pMenuObj[3];
	GameObject* pMenuSelObj[3];
	GameObject* pMenuNoGameObj;
	GameObject* pMenuModObj[2];

	TextureObject* pArrow;
	GameObject* pArrowObj[2];

	/*
	FlatMesh* pTest;
	GameObject* pTestObj;
	
	TextureObject* pTitleTex;
	Model* pTitleModel;
	GameObject* pTitleObj;
	TextObject* pTextNormal;
	TextObject* pTitle;
	*/

	FT_Library ft;

	Camera* pCamera;
};