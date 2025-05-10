#ifndef GAMEOVERSTATE_H
#define GAMEOVERSTATE_H

#include "GameState.h"
#include "MenuState.h"
#include "TextObject.h"
#include "TextureObject.h"
#include "shader_s.h"
#include <ft2build.h>
#include FT_FREETYPE_H

// Specializzazione di GameState per lo stato di fine partita (Game Over)
class GameOverState : public GameState
{
public:
    GameOverState(StateManager* manager, GLFWwindow* window, irrklang::ISoundEngine* engine);
    ~GameOverState();

    void ProcessInput() override;
    void Render() override;
    void EnterState() override;

private:
    FT_Library ft;
    GLFWwindow* pWindow;
    
    StateManager* pStateManager;
    irrklang::ISoundEngine* pSoundEngine;
    TextObject* pTextRenderer;
    TextObject* pTextNormal;

    Shader* pTextShader;

    int CurrentSelection;

    // The player validated the current selection
    void SelectionRestart();

};

#endif  // GAMEOVERSTATE_H

