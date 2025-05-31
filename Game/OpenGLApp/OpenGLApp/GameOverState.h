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
    ~GameOverState();

    void ProcessInput();
    void Render();
    void EnterState();
    void LeaveState();

    // Returns the single instance (-> singleton)
    static GameOverState* GetInstance(StateManager* manager, GLFWwindow* window, irrklang::ISoundEngine* engine);


protected:
    GameOverState(StateManager* manager, GLFWwindow* window, irrklang::ISoundEngine* engine);


private:
    FT_Library ft;
 
    TextObject* pTextRenderer;
    TextObject* pTextNormal;

    Shader* pTextShader;
    Shader* pShader;

};

#endif  // GAMEOVERSTATE_H

