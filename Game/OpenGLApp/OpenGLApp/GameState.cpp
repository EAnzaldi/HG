#include "GameState.h"
#include "StateManager.h" //forward delaration

const glm::vec3 GameState::lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
const glm::vec3 GameState::lightPosition = glm::vec3(0.0f, 0.0f, 2.0f);
//glm::vec3 lightPosition(0.0f, -0.75f, 1.2f);
Camera* GameState::pCamera = new Camera(glm::vec3(0.0f, 0.0f, 0.5f));

GameState::GameState(StateManager* manager, GLFWwindow* window, irrklang::ISoundEngine* engine) : Manager(manager), Window(window), Engine(engine)
{
    // build and compile our shader zprogram
    // -------------------------------------
    pShader = new Shader("resources/shaders/shader.vs", "resources/shaders/shader.fs");
    pTextShader = new Shader("resources/shaders/shader_text.vs", "resources/shaders/shader_text.fs");
    pEnlightenedShader = new Shader("resources/shaders/enlightened_object_shader.vs", "resources/shaders/enlightened_object_shader.fs");
    pSpriteShader = new Shader("resources/shaders/shader_sprite.vs", "resources/shaders/shader_sprite.fs");


    // setup delle uniform delle shader che non cambieranno nel ciclo di rendering
    glm::mat4 view = pCamera->GetViewMatrix();

    pShader->use();
    pShader->setMat4("view", view);

    // Shader sprite
    pSpriteShader->use();
    pSpriteShader->setMat4("view", view);

    //Shader per materiale metallico
    pEnlightenedShader->use();
    pEnlightenedShader->setMat4("view", view);
    pEnlightenedShader->setVec3("viewPos", pCamera->Position);
    ShaderManager::SetLight(*pEnlightenedShader, lightPosition, lightColor);

    TextureObject* pMouseTex = new TextureObject("resources/textures/cross3.png");
    Mouse = new MouseObject(window, pMouseTex, glm::vec3(75.0f, 75.0f, 0.1f));
}

GameState::~GameState() {
    delete pShader;
    delete pTextShader;
    delete pEnlightenedShader;
    delete pSpriteShader;
}

void GameState::ChangeState(GameState* newState)
{
	Engine->stopAllSounds();
	Manager->ChangeState(newState);
}