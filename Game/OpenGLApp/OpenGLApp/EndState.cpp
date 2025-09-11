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

    pText = new TextObject(ft, "resources/fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf");

    pCamera = new Camera(glm::vec3(0.0f, 0.0f, 0.5f));
    glm::mat4 view = pCamera->GetViewMatrix();
    pSpriteShader->use();
    pSpriteShader->setMat4("view", view);

    //int fbWidth, fbHeight;
    //glfwGetFramebufferSize(Window, &fbWidth, &fbHeight);
    pBackgroundTex = new TextureObject("resources/textures/game_over3.png");
    pBackground = new GameObject(glm::vec2(SCR_WIDTH_F / 2.0f, SCR_HEIGHT_F / 2.0f), pBackgroundTex->getSize(), pBackgroundTex, 0);

    pWitchTex = new TextureObject("resources/textures/witch2.png");

    glm::vec2 posPixels = glm::vec2(0.15 * SCR_WIDTH_F + pWitchTex->getWidth() / 2.0f, SCR_HEIGHT_F / 2.0f);
    glm::vec2 posNDC = GameObject::NDCPosition(glm::vec2(posPixels));
    glm::vec3 sizePixels = glm::vec3(pWitchTex->Width, pWitchTex->Height, 1.0f);
    glm::vec3 sizeNDC = GameObject::NDCSize(sizePixels);

    pWitch = new MovingObject(posPixels, sizePixels, pWitchTex, 0, glm::vec2(0.0f, 0.0f), 0);
    //pWitch = new MovingObject(posPixels, pWitchTex->getSize(), pWitchTex, 0, glm::vec2(0.0f, 0.0f), 1);
    //pTexEnemy = new TextureObject("resources/textures/slime2-mod.png");
    //pEnemy = new GameObject(glm::vec2(0.0f, 0.0f), glm::vec3(0.13f, 0.13f, 0.1f), pTexEnemy, 0);
}

EndState::~EndState()
{
    delete pText;
    delete pBackgroundTex;
    delete pBackground;

    FT_Done_FreeType(ft);
}

EndState* EndState::GetInstance(StateManager* manager, GLFWwindow* window, irrklang::ISoundEngine* engine)
{
    static EndState Instance(manager, window, engine);
    return &Instance;
}
void EndState::EnterState()
{
    //glm::vec2 prova1 = glm::vec2(0.0f, 0.0f);
    //glm::vec2 prova2 = glm::vec2(0.0f, pWitch->Size.y/2.0f);
    pWitch->SetRotation(5.0f, GameObject::axisZ, 0.2f);
    pWitch->totalSwings = 3;
    oscillate = true;

    // musica di sottofondo (non loop)
    //ost = Engine->play2D("resources/sounds/cleyron_xavier_no_hope.wav", false, false, true);
    Engine->play2D("resources/sounds/crazy_laugh.wav");
}

void EndState::LeaveState()
{
    //ost->stop();
}

void EndState::ProcessInput()
{
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;

    if (deltaTime > 0.1f)// Ignora delta troppo grandi
        deltaTime = 0.0f;

    lastFrame = currentFrame;

    if (glfwGetKey(Window, GLFW_KEY_ENTER) == GLFW_PRESS) {
        ChangeState(MenuState::GetInstance(Manager, Window, Engine));
    }
}
void EndState::ProcessEvents() {
    if (oscillate) {
        oscillate = pWitch->Oscillate(deltaTime);
    }
    else {

    }
}
void EndState::MouseClick(double xpos, double ypos, int button, int action, int mods)
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

    glDisable(GL_DEPTH_TEST);

    ShaderManager::SetProjection(*pSpriteShader, Window, ProjectionType::Pixels);
    pBackground->Render(*pSpriteShader);


    //ShaderManager::SetProjection(*pSpriteShader, Window, ProjectionType::NDC);
    pWitch->Render(*pSpriteShader);

    glEnable(GL_DEPTH_TEST);

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

