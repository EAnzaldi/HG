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
#include "random.h"

#define TOTENEM 2

class PlayState : public GameState
{
public:
	~PlayState();

	// Implementation of specific events
	void ProcessInput();
	void ProcessEvents();
	void UpdateTime(long currentTime);
	void Render();
	void EnterState();
	void LeaveState();

	void Reset();

	const double start = 99;// tempo massimo per livello

	// Returns the single instance (-> singleton)
	static PlayState* GetInstance(StateManager* manager, GLFWwindow* window, irrklang::ISoundEngine* engine);

protected:
	PlayState(StateManager* manager, GLFWwindow* window, irrklang::ISoundEngine* engine);


private:
	void RenderStats();

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
	//std::vector<Enemy*> pEnemies = std::vector<Enemy*>(TOTENEM, nullptr);
	std::vector<Enemy*> pEnemies;

	int nEnemies;

	Model* pCubeModel;
	Model* pCauldronModel;
	Model* pSlimeModel;
	Model* pBackgroundModel;

	GameObject* pCauldron_right;
	GameObject* pCauldron_left;
	GameObject* pBackground;

	FlatMesh* pTest;
	std::vector<GameObject> tests;


	glm::vec2 positions[8];
	glm::vec3 sizes[8];

	glm::vec2 positionsTest[8];
	glm::vec2 sizesTest[8];

	// Pointer to the current player
	Player* pPlayer;
	Enemy* pEnemy;

	// The current level
	int CurrentLevel;
	// The current score
	long CurrentScore;

	Camera* pCamera;

	// build and compile our shader zprogram
	// -------------------------------------
	Shader* pShader;
	Shader* pTextShader;
	Shader* pEnlightenedShader;
	Shader* pEnlightenedTexturedShader;
	Shader* pSpriteShader;

	// The background image
	//TImagePtr m_pBackgroundImg;

	TextureObject* pTexPlatforms;
	TextureObject* pTexPlayer;
	TextureObject* pTexEnemy;
	TextureObject* pTexSlime;
	TextureObject* pTexBackground;

	FT_Library ft;
	TextObject* pText;

	// Gestione frame
	double lastFrame;
	double deltaTime;

	//Store time when game pauses
	int currentTime;//countdown
	double startTime;
	double totalPauseTime;
	double startPauseTime;

	//Store time per enemies' spawn
	

};

#endif
