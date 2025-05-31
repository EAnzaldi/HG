#include "EndState.h"
#include "StateManager.h"
#include "MenuState.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

EndState::EndState(StateManager* manager, GLFWwindow* window, irrklang::ISoundEngine* engine)
    : GameState(manager, window, engine)
{

    if (FT_Init_FreeType(&ft)) {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }

    pTextNormal = new TextObject(ft, "resources/fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf");

    pShader = new Shader("shader.vs", "shader.fs");
    pTextShader = new Shader("shader_text.vs", "shader_text.fs");

    pShader->use();
}


EndState::~EndState()
{
    delete pTextRenderer;
    delete pTextShader;
    FT_Done_FreeType(ft);
}

EndState* EndState::GetInstance(StateManager* manager, GLFWwindow* window, irrklang::ISoundEngine* engine)
{
    static EndState Instance(manager, window, engine);
    return &Instance;
}

void EndState::EnterState()
{
    
}

void EndState::LeaveState()
{
    
}


void EndState::ProcessInput()
{
    if (glfwGetKey(Window, GLFW_KEY_ENTER) == GLFW_PRESS) {
        ChangeState(MenuState::GetInstance(Manager, Window, Engine));
    }
}

void EndState::Render()
{
    //std::cout << "Rendering EndState" << std::endl;

    glm::vec3 TextColor;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (Status == GameStatus::GameOver) {
        // Pulisce lo schermo
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        //glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        TextColor = { 255.0f, 255.0f, 255.0f };
        // "Game Over" centrato e in alto
        pTextNormal->Render(*pTextShader, "Game Over", SCR_WIDTH / 2 - 300.0f, SCR_HEIGHT / 2 + 100.0f, 2.0f, TextColor);
        // "Premi INVIO per ricominciare" centrato sotto
        pTextNormal->Render(*pTextShader, "Premi INVIO per ricominciare", SCR_WIDTH / 2 - 300.0f, SCR_HEIGHT / 2 - 50.0f, 1.2f, TextColor);
    }
    else if (Status == GameStatus::Victory) {
        // Pulisce lo schermo
        glClearColor(255.0f, 255.0f, 255.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        TextColor = { 0.0f, 0.0f, 0.0f };
        // "Victory" centrato e in alto
        pTextNormal->Render(*pTextShader, "Victory", SCR_WIDTH / 2 - 300.0f, SCR_HEIGHT / 2 + 100.0f, 2.0f, TextColor);
        // "Premi INVIO per ricominciare" centrato sotto
        pTextNormal->Render(*pTextShader, "Premi INVIO per ricominciare", SCR_WIDTH / 2 - 300.0f, SCR_HEIGHT / 2 - 50.0f, 1.2f, TextColor);
    }    

}

