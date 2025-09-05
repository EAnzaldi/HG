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

    pCamera = new Camera(glm::vec3(0.0f, 0.0f, 0.5f));
    glm::mat4 view = pCamera->GetViewMatrix();
    pSpriteShader->use();
    pSpriteShader->setMat4("view", view);
    ShaderManager::SetProjection(*pSpriteShader, Window, ProjectionType::Pixels);

    int fbWidth, fbHeight;
    glfwGetFramebufferSize(Window, &fbWidth, &fbHeight);
    pBackgroundTex = new TextureObject("resources/textures/game_over2.png");
    pBackground = new GameObject(glm::vec2(fbWidth/2.0f, fbHeight/2.0f), pBackgroundTex->getSize(), pBackgroundTex, 0);
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

void EndState::MouseClick(int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        ChangeState(MenuState::GetInstance(Manager, Window, Engine));
    }
}

void EndState::Render()
{
    //std::cout << "Rendering EndState" << std::endl;

    glm::vec3 TextColor;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    pBackground->Render(*pSpriteShader);

    /*
    if (Status == GameStatus::GameOver) {
        // Pulisce lo schermo
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        //glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        TextColor = { 255.0f, 255.0f, 255.0f };
        // "Game Over" centrato e in alto
        pTextNormal->Render(*pTextShader, "Game Over", SCR_WIDTH / 2 - 300.0f, SCR_HEIGHT / 2 + 100.0f, 2.0f, TextColor, Alignment::Left);
        // "Premi INVIO per ricominciare" centrato sotto
        pTextNormal->Render(*pTextShader, "Premi INVIO per ricominciare", SCR_WIDTH / 2 - 300.0f, SCR_HEIGHT / 2 - 50.0f, 1.2f, TextColor, Alignment::Left);
    }
    else if (Status == GameStatus::Victory) {
        // Pulisce lo schermo
        glClearColor(255.0f, 255.0f, 255.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        TextColor = { 0.0f, 0.0f, 0.0f };
        // "Victory" centrato e in alto
        pTextNormal->Render(*pTextShader, "Victory", SCR_WIDTH / 2 - 300.0f, SCR_HEIGHT / 2 + 100.0f, 2.0f, TextColor, Alignment::Left);
        // "Premi INVIO per ricominciare" centrato sotto
        pTextNormal->Render(*pTextShader, "Premi INVIO per ricominciare", SCR_WIDTH / 2 - 300.0f, SCR_HEIGHT / 2 - 50.0f, 1.2f, TextColor, Alignment::Left);
    }    
    */

}

