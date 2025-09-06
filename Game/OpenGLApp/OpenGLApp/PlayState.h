#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <unordered_map>
#include <iomanip>

#include "Camera.h"
#include "TextObject.h"
#include "TextureObject.h"
#include "model.h"
#include "GameState.h"
#include "MenuState.h"
#include "EndState.h"
#include "Player.h"
#include "Candy.h"
#include "ScoreState.h"

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

	void MouseMoving(double xpos, double ypos) override;
	void MouseClick(int button, int action, int mods) override;

	void Reset();
	void ResetLevel() { CurrentLevel[Multiplayer] = StartLevel; };

	void SwitchMode() { Multiplayer = !Multiplayer; };
	bool IsMultiplayer() { return Multiplayer; };
	//bool IsMultiplayerUnlocked() { return MultiplayerUnlocked; };
	int GetLvl() { return CurrentLevel[Multiplayer]; };

	GameStatus GetStatus() { return Status[Multiplayer]; }

	static bool MultiplayerUnlocked;
	static bool TeleportUnlocked;

	GameStatus Status[2];

	int CurrentLevel[2];// The current level
	int StartLevel = 1;// The initial level
	//int StoryLevel;
	//bool CompletedLevels[4] = { false, false, false, false };

	const double start = 99;// tempo massimo per livello

	static const int scoreMalus = 10;
	static const int scoreEnemy = 50;
	static const int scoreBonus = 60;
	static const int scoreTelep = 100;
	static const int scoreTime = 10; //pt x sec rimasto

	// The current score
	long CurrentScore;
	void AddScore(int score) { CurrentScore += score; };

	std::vector<TextureObject*> pCandiesMesh;
	//std::unordered_map<CandyType*, TextureObject*> typeToTextureMap;
	std::vector<CandyType*> pCandyTypes;
	std::vector<int> pProbabilities;

	TextureObject* pTexGretel;
	TextureObject* pTexHansel;
	TextureObject* pTexPlatforms;
	TextureObject* pTexSlime;

	std::vector<int> GretelCandyStats;
	std::vector<int> HanselCandyStats;
	int GretelKills;
	int HanselKills;

	glm::vec3 candySize;

	int remainingTime;//countdown

	// Returns the single instance (-> singleton)
	static PlayState* GetInstance(StateManager* manager, GLFWwindow* window, irrklang::ISoundEngine* engine);

protected:
	PlayState(StateManager* manager, GLFWwindow* window, irrklang::ISoundEngine* engine);


private:
	void RenderStats();
	void ProcessInputPlayer(Player* pPlayer, unsigned int UP, unsigned int DOWN, unsigned int LEFT, unsigned int RIGHT);
	void CheckEndGame();

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

	std::vector<GameObject*> platforms;
	std::vector<Enemy*> pEnemies;
	std::vector<GameObject*> pCauldrons;
	std::vector<Candy*> pCandies;
	GameObject* pKeys[2];

	GameObject* pHearts[2];
	GameObject* pSlimeUI;
	GameObject* pKeysUI[2];

	int nEnemiesKilled;
	int nEnemiesAlive;
	int spawnPlace;

	int nKeys;

	Model* pCubeModel;
	Model* pCauldronModel;
	Model* pSlimeModel;
	//Model* pBackgroundModel;

	//GameObject* pBackground;

	//FlatMesh* pTest;
	//std::vector<GameObject> tests;

	TextureObject* pKeysTex[2];

	TextureObject* pHeartsTex[2];

	glm::vec2 positions[8];
	glm::vec3 sizes[8];

	glm::vec2 positionsTest[8];
	glm::vec2 sizesTest[8];

	// Pointer to the current player
	Player* pGretel;
	Player* pHansel;

	bool Multiplayer;

	Camera* pCamera;

	//TextureObject* pTexPlayer;
	//TextureObject* pTexEnemy;
	//TextureObject* pTexBackground;

	FT_Library ft;
	TextObject* pText;

	// Gestione frame
	double lastFrame;
	double deltaTime;

	//Store time when game pauses
	int currentTime;//count-up
	double startTime;
	double totalPauseTime;
	double startPauseTime;
	int spawnTime;
	double lastSpawnTime;

	const float endingDuration = 1.0f;	// Piccolo delay di fine gioco
	float endingTimer = 0.0f;			// Timer dopo il fine gioco
	bool isEnding = false;

	//Sound
	irrklang::ISound* ost;
};