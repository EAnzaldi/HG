#pragma once

#include "GameState.h"
#include "MenuState.h"
#include "TextObject.h"
#include "TextureObject.h"
#include "shader_s.h"
#include <ft2build.h>
#include FT_FREETYPE_H

// Ex GameOverState
// Specializzazione di EndState per lo stato di fine partita (Game Over - Victory)

class EndState : public GameState
{
public:
    ~EndState();

    void ProcessInput();
    void MouseClick(int button, int action, int mods);

    void Render();
    void EnterState();
    void LeaveState();

    // Returns the single instance (-> singleton)
    static EndState* GetInstance(StateManager* manager, GLFWwindow* window, irrklang::ISoundEngine* engine);


protected:
    EndState(StateManager* manager, GLFWwindow* window, irrklang::ISoundEngine* engine);


private:
    FT_Library ft;
 
    TextObject* pTextRenderer;
    TextObject* pTextNormal;

    Shader* pTextShader;
    Shader* pShader;

};
