#ifndef PLAYSTATE_H
#define PLAYSTATE_H

#include "GameState.h"
#include "Player.h"

class PlayState : public GameState
{
public:

	PlayState(StateManager* manager, Player* player);

	// Implementation of specific events
	void ProcessInput(GLFWwindow* window, float deltatime, irrklang::ISoundEngine* engine);
	void UpdateTime(long currentTime);
	void Draw();

	void Reset();
	bool IsGameOver() { return GameOver; }

private:
	// The blocks matrix class
	//CBlocksMatrix* m_pMatrix;
	// The font used to draw text
	//CGameFont* m_pFont;
	// The control in charge of the decreasing 
	// time for the combo score.
	//CComboControl* m_pComboControl;

	// The text controls to display the current
	// information.
	//CTextControl* m_pScoreControl;
	//CTextControl* m_pLevelControl;
	//CTextControl* m_pLinesControl;

	// Pointer to the current player
	Player* pPlayer;
	// The current level
	int CurrentLevel;
	// The current score
	long CurrentScore;

	bool GameOver;

	// The background image
	//TImagePtr m_pBackgroundImg;
};

#endif
