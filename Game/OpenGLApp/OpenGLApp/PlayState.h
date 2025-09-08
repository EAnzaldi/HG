#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <unordered_map>
#include <iomanip>
#include <deque>
#include <tuple>

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
	int GetLvl() { return CurrentLevel[Multiplayer]; };
	GameStatus GetStatus() { return Status[Multiplayer]; }

	// Returns the single instance (-> singleton)
	static PlayState* GetInstance(StateManager* manager, GLFWwindow* window, irrklang::ISoundEngine* engine);

	GameStatus Status[2];

	std::vector<TextureObject*> pCandiesMesh;
	TextureObject* pTexGretel;
	TextureObject* pTexHansel;
	TextureObject* pTexPlatforms;
	TextureObject* pTexSlime;

	std::vector<CandyType*> pCandyTypes;

	std::vector<int> pProbabilities;
	std::vector<int> GretelCandyStats;
	std::vector<int> HanselCandyStats;

	glm::vec3 candySize;

	static bool MultiplayerUnlocked;
	static bool TeleportUnlocked;
	static const int scoreMalus = 100;
	static const int scoreEnemy = 500;
	static const int scoreBonus = 600;
	static const int scoreTelep = 1000;
	static const int scoreTime = 50; //pt x sec rimasto

	const double start = 99;// tempo massimo per livello

	int CurrentLevel[2];// The current level
	int StartLevel = 1;// The initial level

	long CurrentScore;
	int GretelKills;
	int HanselKills;

	int remainingTime;//countdown

protected:
	PlayState(StateManager* manager, GLFWwindow* window, irrklang::ISoundEngine* engine);


private:
	void RenderStats();
	void ProcessInputPlayer(Player* pPlayer, unsigned int UP, unsigned int DOWN, unsigned int LEFT, unsigned int RIGHT);
	void CheckEndGame();

	TextObject* pText;

	TextureObject* pKeysTex[2];
	TextureObject* pHeartsTex[2];

	Model* pCubeModel;
	Model* pCauldronModel;
	Model* pSlimeModel;

	std::vector<GameObject*> platforms;
	std::vector<GameObject*> pCauldrons;
	GameObject* pHearts[2];
	GameObject* pKeysUI[2];
	GameObject* pKeys[2];
	GameObject* pSlimeUI;

	Player* pGretel;
	Player* pHansel;

	std::vector<Enemy*> pEnemies;

	std::vector<Candy*> pCandies;

	bool Multiplayer;

	glm::vec2 positions[8];
	glm::vec3 sizes[8];
	glm::vec2 positionsTest[8];
	glm::vec2 sizesTest[8];

	int nEnemiesKilled;
	int nEnemiesAlive;
	int spawnPlace;

	int nKeys;

	Camera* pCamera;

	FT_Library ft;

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

	const float displayDuration = 1.0f;
	const float displayDurationNonEmpty = 1.0f;
	//float displayTimer = 0.0f;
	bool newDisplay = false;

	//Sound
	irrklang::ISound* ost;
};