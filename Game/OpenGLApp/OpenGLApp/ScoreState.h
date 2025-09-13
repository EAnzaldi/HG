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

    void ProcessInput() override;
    void MouseClick(double xpos, double ypos, int button, int action, int mods) override;

    void Render() override;
    void EnterState() override;
    void LeaveState() override;

    // Returns the single instance (-> singleton)
    static ScoreState* GetInstance(StateManager* manager, GLFWwindow* window, irrklang::ISoundEngine* engine);


protected:
    ScoreState(StateManager* manager, GLFWwindow* window, irrklang::ISoundEngine* engine);


private:

    void ProcessInputPlayer(Player* pPlayer, unsigned int UP, unsigned int DOWN, unsigned int LEFT, unsigned int RIGHT);
    void RenderStats();

    // A pointer to the current active game (if any).
    PlayState* CurrentGame = nullptr;

    TextObject* pText = nullptr;

    TextureObject* pLvlsTex[2];

    Model* pCageModel = nullptr;

    std::vector<GameObject*> solidsGretel;
    std::vector<GameObject*> solidsHansel;
    GameObject* pLvls[2] = { nullptr, nullptr };
    GameObject* pFloor = nullptr;
    MovingObject* pCage = nullptr;
    GameObject* pCandy = nullptr;
    GameObject* pSlime = nullptr;

    Player* pGretel = nullptr;
    Player* pHansel = nullptr;

    // Gestione frame
    double lastFrame = 0.0f;
    double deltaTime = 0.0f;

    //Animazione gabbia
    glm::vec2 pivotCage = glm::vec2(0.0f, 0.0f);
    glm::vec2 pivotHansel = glm::vec2(0.0f, 0.0f);

    bool oscillate = true;
    int nPauses = 0;
    bool HanselFree = false;
    bool cageFall = false;

    //Animazione uscita
    float xtarget = 1.1f;
    
    //Segnala che si è in uno stato sicuro dell'animazione
    bool end = false;

    const float timerDuration = 0.5f;
    float timer = 0.0f;

    //Sound
    irrklang::ISound* ost = nullptr;

    FT_Library ft;
};

