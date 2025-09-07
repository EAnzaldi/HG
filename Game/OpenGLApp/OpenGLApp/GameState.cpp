#include "GameState.h"
#include "StateManager.h" //forward delaration

GameState::GameState(StateManager* manager, GLFWwindow* window, irrklang::ISoundEngine* engine) : Manager(manager), Window(window), Engine(engine)
{
    // build and compile our shader zprogram
    // -------------------------------------
    pShader = new Shader("resources/shaders/shader.vs", "resources/shaders/shader.fs");
    pTextShader = new Shader("resources/shaders/shader_text.vs", "resources/shaders/shader_text.fs");
    pEnlightenedShader = new Shader("resources/shaders/enlightened_object_shader.vs", "resources/shaders/enlightened_object_shader.fs");
    pSpriteShader = new Shader("resources/shaders/shader_sprite.vs", "resources/shaders/shader_sprite.fs");

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