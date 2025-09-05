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

    pTextNormal = new TextObject(ft, "resources/fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf");

    pLevelsCompletedTex[0] = new TextureObject("resources/textures/level_1_completed.png");
    pLevelsCompletedTex[1] = new TextureObject("resources/textures/level_2_completed.png");

    // caricamento modelli blender
    pCageModel = new Model("resources/models/Cage.obj");

    //Shader per materiale metallico
    pEnlightenedShader->use();
    pEnlightenedShader->setMat4("view", view);
    pEnlightenedShader->setVec3("viewPos", pCamera->Position);

    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
    glm::vec3 lightPosition(0.0f, -0.0f, 2.0f);

    ShaderManager::SetProjection(*pEnlightenedShader, Window, ProjectionType::NDC);
    ShaderManager::SetMaterial(*pEnlightenedShader, ShaderManager::Silver);
    ShaderManager::SetLight(*pEnlightenedShader, lightPosition, lightColor);
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
    int fbWidth, fbHeight;
    glfwGetFramebufferSize(Window, &fbWidth, &fbHeight);
    if (CurrentGame == nullptr)
        CurrentGame = PlayState::GetInstance(Manager, Window, Engine);
    if (pCandy == nullptr)
        pCandy = new GameObject(glm::vec2(0.0f, 0.0f), glm::vec3(0.07f * fbWidth/2.0f, 0.07f * fbHeight/2.0f * getAspect(Window), 0.1f), (TextureObject*) nullptr, 0);
    if (pSlime == nullptr)
        pSlime = new GameObject(glm::vec2(0.0f, 0.0f), glm::vec3(0.13f * CurrentGame->pTexSlime->getAspect() * fbWidth / 2.0f, 0.13f * fbHeight / 2.0f, 0.1f), CurrentGame->pTexSlime, 0);
    int lvl = CurrentGame->CurrentLevel[CurrentGame->IsMultiplayer()] - 1;
    if (pLevelsCompleted[lvl] == nullptr)
        pLevelsCompleted[lvl] = new GameObject(glm::vec2(fbWidth*0.5f, fbHeight*0.85f), pLevelsCompletedTex[lvl]->getSize() * 1.06f, pLevelsCompletedTex[lvl], 0);
    if (pFloor == nullptr) {
        pFloor =  new GameObject(glm::vec2{ 0.0f, -0.95f }, glm::vec3{ 2.0f, 0.1f, 0.2f }, CurrentGame->pTexPlatforms, 1);
        solidsGretel.emplace_back(pFloor);
        solidsHansel.emplace_back(pFloor);

    }
    if (pGretel == nullptr)
        pGretel = new Player(glm::vec2(-0.5f, -0.85f), glm::vec3(0.12f, 0.12f * getAspect(Window) * CurrentGame->pTexGretel->getAspect(), 0.1f), CurrentGame->pTexGretel, 0, PlayerName::Gretel, 3);
    else
        pGretel->Position = glm::vec2(-0.5f, -0.85f);
    if (pHansel == nullptr)
        pHansel = new Player(glm::vec2(0.5f, -0.85f), glm::vec3(0.12f, 0.12f * getAspect(Window) * CurrentGame->pTexHansel->getAspect(), 0.1f), CurrentGame->pTexHansel, 0, PlayerName::Hansel, 3);
    else
        pHansel->Position = glm::vec2(0.5f, -0.85f);
    if (pCage == nullptr) {
        pCage = new GameObject(glm::vec2(0.5f, -0.72f), glm::vec3(0.1f, 0.1f, 0.1f), pCageModel, 0);
        solidsGretel.emplace_back(pCage);
    }
    else if (CurrentGame->GetLvl() == 2 && !CurrentGame->IsMultiplayer()) {
        delete solidsGretel[1];
        solidsGretel.resize(1);
    }
}

void ScoreState::LeaveState()
{

}

void ScoreState::ProcessInput()
{
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;

    if (deltaTime > 0.1f)// Ignora delta troppo grandi
        deltaTime = 0.0f;

    lastFrame = currentFrame;

    if (glfwGetKey(Window, GLFW_KEY_ENTER) == GLFW_PRESS) {
        CurrentGame->CurrentLevel[CurrentGame->IsMultiplayer()]++;
        if (CurrentGame->CurrentLevel[CurrentGame->IsMultiplayer()] < 3) {
            ChangeState(CurrentGame);
            CurrentGame->Reset();//Inizia nuovo livello
        } else {
            ChangeState(MenuState::GetInstance(Manager, Window, Engine));
        }
    }

    if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        ChangeState(MenuState::GetInstance(Manager, Window, Engine));
    }

    if (CurrentGame->GetLvl() == 1) {
        pGretel->Move(deltaTime);
        pGretel->CheckCollisionWithSolids(solidsGretel);
        ProcessInputPlayer(pGretel, GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_A, GLFW_KEY_D);
        if (CurrentGame->IsMultiplayer()) 
            pHansel->Move(deltaTime);
        pHansel->CheckCollisionWithSolids(solidsHansel);
        if (CurrentGame->IsMultiplayer())
            ProcessInputPlayer(pHansel, GLFW_KEY_UP, GLFW_KEY_DOWN, GLFW_KEY_LEFT, GLFW_KEY_RIGHT);
    } else if (CurrentGame->GetLvl() == 2) {
        if (CurrentGame->IsMultiplayer() == false) {
            pGretel->Move(deltaTime);
            pGretel->CheckCollisionWithSolids(solidsGretel);
            pHansel->Move(deltaTime);
            pHansel->CheckCollisionWithSolids(solidsHansel);
            ProcessInputPlayer(pGretel, GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_A, GLFW_KEY_D);
            ProcessInputPlayer(pHansel, GLFW_KEY_UP, GLFW_KEY_DOWN, GLFW_KEY_LEFT, GLFW_KEY_RIGHT);
        }
    }

}
void ScoreState::ProcessInputPlayer(Player* pPlayer, unsigned int UP, unsigned int DOWN, unsigned int LEFT, unsigned int RIGHT)
{

    // movimento orizzontale
    if (glfwGetKey(Window, LEFT) == GLFW_PRESS)
    {
        if (pPlayer->velocity.x > -pPlayer->maxVelocity.x)
            pPlayer->velocity.x -= 0.01f;
    }
    else if (glfwGetKey(Window, RIGHT) == GLFW_PRESS)
    {
        if (pPlayer->velocity.x < pPlayer->maxVelocity.x)
            pPlayer->velocity.x += 0.01f;
    }
    else
        pPlayer->velocity.x = 0.0f;

    // salto
    if (glfwGetKey(Window, UP) == GLFW_PRESS)
    {
        pPlayer->HandleJump(deltaTime, Engine);
    }
    else
    {
        if (pPlayer->isOnGround)
            pPlayer->isPastJumpPeak = true; // Se il tasto non è premuto, non si può più aumentare il salto
    }
}
void ScoreState::MouseClick(int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        CurrentGame->CurrentLevel[CurrentGame->IsMultiplayer()]++;
        if (CurrentGame->CurrentLevel[CurrentGame->IsMultiplayer()] < 3) {
            ChangeState(CurrentGame);
            CurrentGame->Reset();//Inizia nuovo livello
        }
        else {
            ChangeState(MenuState::GetInstance(Manager, Window, Engine));
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

    glDisable(GL_DEPTH_TEST);

    //Oggetti con coordinate NDC
    //--------------------------------------------------------------------------------
    float height = fbHeight * 0.75f;
    float text_width = fbWidth * 0.25;
    float gretel_width = fbWidth * 0.55f;
    float hansel_width = fbWidth * 0.75f;
    float y_indent = fbHeight/20.0f;

    ShaderManager::SetProjection(*pSpriteShader, Window, ProjectionType::Pixels);

    pLevelsCompleted[CurrentGame->CurrentLevel[CurrentGame->IsMultiplayer()] - 1]->Render(*pSpriteShader);

    pTextNormal->Render(*pTextShader, "STATS", text_width, height, 1.0f, TextColor, Alignment::Left);
    
    height -= y_indent;
    pTextNormal->Render(*pTextShader, "Gretel", gretel_width, height, 1.0f, TextColor, Alignment::Right);
    if(CurrentGame->IsMultiplayer())
        pTextNormal->Render(*pTextShader, "Hansel", hansel_width, height, 1.0f, TextColor, Alignment::Right);

    pSlime->Position = glm::vec2(text_width + pSlime->Size.x / 2.0f, height - pSlime->Size.y / 2.0f - y_indent / 2.0f);
    pSlime->Render(*pSpriteShader);
    pTextNormal->Render(*pTextShader, std::to_string(CurrentGame->GretelKills), gretel_width, height - pSlime->Size.y - y_indent / 3.0f, 1.0f, TextColor, Alignment::Right);
    if (CurrentGame->IsMultiplayer()) {
        pTextNormal->Render(*pTextShader, std::to_string(CurrentGame->HanselKills), hansel_width, height - pSlime->Size.y - y_indent / 3.0f, 1.0f, TextColor, Alignment::Right);
    }
    height -= (pSlime->Size.y + y_indent / 2.0f);

    for (int i = 0; i < CurrentGame->pCandiesMesh.size(); i++) {
        bool draw = false;
        if (CurrentGame->GretelCandyStats[i] != 0) {
            pTextNormal->Render(*pTextShader, std::to_string(CurrentGame->GretelCandyStats[i]), gretel_width, height - pCandy->Size.y - y_indent / 3.0f, 1.0f, TextColor, Alignment::Right);
            draw = true;
        }
        if (CurrentGame->IsMultiplayer() && CurrentGame->HanselCandyStats[i] != 0) {
            pTextNormal->Render(*pTextShader, std::to_string(CurrentGame->HanselCandyStats[i]), hansel_width, height - pCandy->Size.y - y_indent / 3.0f, 1.0f, TextColor, Alignment::Right);
            draw = true;
        }
        if (draw) {
            //if (i < CurrentGame->pCandiesMesh.size() - 1)
            pCandy->Position = glm::vec2(text_width + pCandy->Size.x / 2.0f, height - pCandy->Size.y/2.0f - y_indent / 2.0f);
            height -= (pCandy->Size.y + y_indent / 2.0f);
            pCandy->Texture = CurrentGame->pCandiesMesh[i];
            pCandy->Render(*pSpriteShader);
        }

    }
    height -= y_indent;
    std::string time = "TIME LEFT: " + std::to_string(CurrentGame->remainingTime);
    pTextNormal->Render(*pTextShader, time, text_width, height, 1.0f, TextColor, Alignment::Left);
    height -= y_indent;
    std::string score = "SCORE: " + std::to_string(CurrentGame->CurrentScore);
    pTextNormal->Render(*pTextShader, score, text_width, height, 1.0f, TextColor, Alignment::Left);


    //Oggetti con coordinate pixel
    //--------------------------------------------------------------------------------
    ShaderManager::SetProjection(*pSpriteShader, Window, ProjectionType::NDC);

    pFloor->Render(*pSpriteShader);
    if (CurrentGame->GetLvl() == 1 || (CurrentGame->GetLvl() >= 2 && !CurrentGame->IsMultiplayer())) {
        pGretel->Render(*pSpriteShader);
        pHansel->Render(*pSpriteShader);
    }

    glEnable(GL_DEPTH_TEST);

    //Oggetti 3D
    //--------------------------------------------------------------------------------
    if(CurrentGame->GetLvl() == 1 && !CurrentGame->IsMultiplayer())
        pCage->Render(*pEnlightenedShader);
}