#ifndef PLAYSTATE_H
#define PLAYSTATE_H

#include "GameState.h"
#include "Player.h"
#include "Camera.h"
#include "TextObject.h"
#include "TextureObject.h"
#include "model.h"
#include <glad/glad.h>
#include <glm/glm.hpp>

class PlayState : public GameState
{
public:
	~PlayState();

	// Implementation of specific events
	void ProcessInput();
	void UpdateTime(long currentTime);
	void Render();

	void Reset();
	bool IsGameOver() { return GameOver; }

	const double start = 99.0f;// tempo massimo per livello

	// Returns the single instance (-> singleton)
	static PlayState* GetInstance(StateManager* manager, GLFWwindow* window, irrklang::ISoundEngine* engine);

protected:
	PlayState(StateManager* manager, GLFWwindow* window, irrklang::ISoundEngine* engine);


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


	std::vector<GameObject> platforms;
	//std::vector<Enemy> enemies;

	Model* pCubeModel;
	Model* pCauldronModel;

	GameObject* pCauldron_right;
	GameObject* pCauldron_left;

	glm::vec2 positions[8];
	glm::vec3 sizes[8];

	// Pointer to the current player
	Player* pPlayer;
	Enemy* pEnemy;

	// The current level
	int CurrentLevel;
	// The current score
	long CurrentScore;

	bool GameOver;

	Camera* pCamera;

	// build and compile our shader zprogram
	// -------------------------------------
	Shader* pShader;
	Shader* pTextShader;
	Shader* pEnlightenedShader;

	// The background image
	//TImagePtr m_pBackgroundImg;

	TextureObject* pTexPlatforms;
	TextureObject* pTexPlayer;
	TextureObject* pTexEnemy;

	FT_Library ft;
	TextObject* pText;

	// Gestione frame
	float lastFrame;
	float deltaTime;	
};

#endif
