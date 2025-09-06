#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include "shader_s.h"
#include "GameState.h"
#include "MenuState.h"
#include "TextObject.h"
#include "TextureObject.h"

class PlayState; //forward declaration per evitare loop di inclusioni

class ScoreState : public GameState
{
public:
    ~ScoreState();

    void ProcessInput();
    void MouseClick(int button, int action, int mods);

    void Render();
    void EnterState();
    void LeaveState();

    // Returns the single instance (-> singleton)
    static ScoreState* GetInstance(StateManager* manager, GLFWwindow* window, irrklang::ISoundEngine* engine);


protected:
    ScoreState(StateManager* manager, GLFWwindow* window, irrklang::ISoundEngine* engine);


private:

    void ProcessInputPlayer(Player* pPlayer, unsigned int UP, unsigned int DOWN, unsigned int LEFT, unsigned int RIGHT);

    // A pointer to the current active game (if any).
    PlayState* CurrentGame;

    TextObject* pTextNormal;

    TextureObject* pLevelsCompletedTex[2];

    Model* pCageModel;

    std::vector<GameObject*> solidsGretel;
    std::vector<GameObject*> solidsHansel;
    GameObject* pLevelsCompleted[2];
    GameObject* pFloor;
    GameObject* pCage;
    GameObject* pCandy;
    GameObject* pSlime;

    Player* pGretel;
    Player* pHansel;

    // Gestione frame
    double lastFrame;
    double deltaTime;

    //Sound
    irrklang::ISound* ost;

    FT_Library ft;

    Camera* pCamera;
};

