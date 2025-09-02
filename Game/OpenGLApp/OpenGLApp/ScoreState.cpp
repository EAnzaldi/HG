#include "ScoreState.h"
#include "PlayState.h"

ScoreState::ScoreState(StateManager* manager, GLFWwindow* window, irrklang::ISoundEngine* engine)
    : GameState(manager, window, engine)
{

    if (FT_Init_FreeType(&ft)) {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }

    pCamera = new Camera(glm::vec3(0.0f, 0.0f, 0.5f));
    glm::mat4 view = pCamera->GetViewMatrix();
    pSpriteShader->use();
    pSpriteShader->setMat4("view", view);

    ShaderManager::SetProjection(*pSpriteShader, window, ProjectionType::Pixels);

    pTextNormal = new TextObject(ft, "resources/fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf");
}


ScoreState::~ScoreState()
{

    FT_Done_FreeType(ft);
}

ScoreState* ScoreState::GetInstance(StateManager* manager, GLFWwindow* window, irrklang::ISoundEngine* engine)
{
    static ScoreState Instance(manager, window, engine);
    return &Instance;
}

void ScoreState::EnterState()
{
    if (CurrentGame == nullptr)
        CurrentGame = PlayState::GetInstance(Manager, Window, Engine);
    if (pCandy == nullptr) {
        int fbWidth, fbHeight;
        glfwGetFramebufferSize(Window, &fbWidth, &fbHeight);
        pCandy = new GameObject(glm::vec2(0.0f, 0.0f), glm::vec3(0.07f * fbWidth/2.0f, 0.07f * fbHeight/2.0f * getAspect(Window), 0.1f), (TextureObject*) nullptr, 0);
    }
        
}

void ScoreState::LeaveState()
{
}


void ScoreState::ProcessInput()
{
    if (glfwGetKey(Window, GLFW_KEY_ENTER) == GLFW_PRESS) {       
        CurrentGame->CurrentLevel++;
        if (CurrentGame->CurrentLevel < 3) {
            ChangeState(CurrentGame);
            CurrentGame->Reset();//Inizia nuovo livello
        }      
    }
}

void ScoreState::MouseClick(int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        CurrentGame->CurrentLevel++;
        if (CurrentGame->CurrentLevel < 3) {
            ChangeState(CurrentGame);
            CurrentGame->Reset();//Inizia nuovo livello
        }
    }
}

void ScoreState::Render()
{
    int fbWidth, fbHeight;
    glfwGetFramebufferSize(Window, &fbWidth, &fbHeight);

    glm::vec3 TextColor = { 255.0f, 255.0f, 255.0f };

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float height = fbHeight * 0.7f;

    float text_width = fbWidth * 0.1f;
    float gretel_width = fbWidth * 0.4f;
    float hansel_width = fbWidth * 0.6f;

    float y_indent = fbHeight / 12.0f;

    pTextNormal->Render(*pTextShader, "STATS", text_width, height, 1.0f, TextColor, Alignment::Left);
    height -= y_indent;

    pTextNormal->Render(*pTextShader, "Gretel", gretel_width, height, 1.0f, TextColor, Alignment::Right);
    if(CurrentGame->IsMultiplayer())
        pTextNormal->Render(*pTextShader, "Hansel", hansel_width, height, 1.0f, TextColor, Alignment::Right);
    height -= y_indent;

    pTextNormal->Render(*pTextShader, std::to_string(CurrentGame->GretelKills), gretel_width, height, 1.0f, TextColor, Alignment::Right);
    if (CurrentGame->IsMultiplayer()) {
        pTextNormal->Render(*pTextShader, std::to_string(CurrentGame->HanselKills), hansel_width, height, 1.0f, TextColor, Alignment::Right);
    }
    height -= y_indent;

    for (int i = 0; i < CurrentGame->pCandiesMesh.size(); i++) {
        pCandy->Position = glm::vec2(text_width+pCandy->Size.x/2.0f, height + pCandy->Size.y / 2.0f);
        pCandy->Texture = CurrentGame->pCandiesMesh[i];
        pCandy->Render(*pSpriteShader);
        if (CurrentGame->GretelCandyStats[i] != 0) {
            pTextNormal->Render(*pTextShader, std::to_string(CurrentGame->GretelCandyStats[i]), gretel_width, height, 1.0f, TextColor, Alignment::Right);
            if (!CurrentGame->IsMultiplayer())
                height -= pCandy->Size.y;
        }
        if (CurrentGame->IsMultiplayer() && CurrentGame->HanselCandyStats[i] != 0) {
            pTextNormal->Render(*pTextShader, std::to_string(CurrentGame->HanselCandyStats[i]), hansel_width, height, 1.0f, TextColor, Alignment::Right);
            height -= pCandy->Size.y;
        }
    }
    height -= y_indent;
    std::string time = "TIME LEFT: " + std::to_string(CurrentGame->remainingTime);
    pTextNormal->Render(*pTextShader, time, text_width, height, 1.0f, TextColor, Alignment::Left);


}