#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include "shader_s.h"
#include "GameState.h"
#include "MenuState.h"
#include "TextObject.h"
#include "TextureObject.h"
#include "GameObject.h"
#include "MovingObject.h"
#include "Enemy.h"

// Ex GameOverState
// Specializzazione di EndState per lo stato di fine partita (Game Over - Victory)

class EndState : public GameState
{
public:
    ~EndState();

    void ProcessInput() override;
    void ProcessEvents() override;

    void MouseClick(int button, int action, int mods) override;

    void Render() override;
    void EnterState() override;
    void LeaveState() override;

    // Returns the single instance (-> singleton)
    static EndState* GetInstance(StateManager* manager, GLFWwindow* window, irrklang::ISoundEngine* engine);


protected:
    EndState(StateManager* manager, GLFWwindow* window, irrklang::ISoundEngine* engine);


private:

    TextObject* pText;

    TextureObject* pBackgroundTex;
    TextureObject* pWitchTex;

    GameObject* pBackground;
    MovingObject* pWitch;

    TextureObject* pTexEnemy;
    GameObject* pEnemy;

    //Sound
    irrklang::ISound* ost;

    FT_Library ft;

    Camera* pCamera;

    // Gestione frame
    double lastFrame;
    double deltaTime;
};
