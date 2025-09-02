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
    FT_Library ft;

    // A pointer to the current active game (if any).
    PlayState* CurrentGame;

    TextObject* pTextNormal;

    GameObject* pCandy;

    Camera* pCamera;
};

