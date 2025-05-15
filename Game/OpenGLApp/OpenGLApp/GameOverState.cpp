#include "GameOverState.h"
#include "StateManager.h"
#include "MenuState.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#define RESTART 0

GameOverState::GameOverState(StateManager* manager, GLFWwindow* window, irrklang::ISoundEngine* engine)
    : GameState(manager, window, engine), CurrentSelection(RESTART)
{

    if (FT_Init_FreeType(&ft)) {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }

    pTextNormal = new TextObject(ft, "resources/fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf");

    pTextShader = new Shader("shader_text.vs", "shader_text.fs");
}


GameOverState::~GameOverState()
{
    delete pTextRenderer;
    delete pTextShader;
    FT_Done_FreeType(ft);
}

GameOverState* GameOverState::GetInstance(StateManager* manager, GLFWwindow* window, irrklang::ISoundEngine* engine)
{
    static GameOverState Instance(manager, window, engine);
    return &Instance;
}

void GameOverState::EnterState()
{
    // Checks whether there is a current game active
    CurrentSelection = 0;
}

void GameOverState::ProcessInput()
{
    if (glfwGetKey(Window, GLFW_KEY_ENTER) == GLFW_PRESS)
        SelectionRestart();
}

void GameOverState::Render()
{
    std::cout << "Rendering GameOverState" << std::endl;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // Pulisce lo schermo
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glm::vec3 WhiteColor = { 255.0f, 255.0f, 255.0f };

    // "Game Over" centrato e in alto
    pTextNormal->Render(*pTextShader, "Game Over", SCR_WIDTH / 2, SCR_HEIGHT / 2 + 100.0f, 2.0f, WhiteColor);

    // "Premi INVIO per ricominciare" centrato sotto
    pTextNormal->Render(*pTextShader, "Premi INVIO per ricominciare", SCR_WIDTH / 2, SCR_HEIGHT / 2 - 50.0f, 1.2f, WhiteColor);

    glfwSwapBuffers(Window);
}

void GameOverState::SelectionRestart()
{
   // Manager->ChangeState(new MenuState(Manager, Window, Engine));
    ChangeState(MenuState::GetInstance(Manager, Window, Engine));
}

