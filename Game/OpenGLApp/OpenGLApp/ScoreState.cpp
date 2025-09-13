#include "ScoreState.h"
#include "PlayState.h"

#define NPAUSE 2

bool DrawTeleportUnlocked = false;
bool DrawMultiplayerUnlocked = false;

ScoreState::ScoreState(StateManager* manager, GLFWwindow* window, irrklang::ISoundEngine* engine)
    : GameState(manager, window, engine)
{

    if (FT_Init_FreeType(&ft)) {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }

    CurrentGame = PlayState::GetInstance(Manager, Window, Engine);

    // Caricamento font
    pText = new TextObject(ft, "resources/fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf");

    // Caricamento textures
    pLvlsTex[0] = new TextureObject("resources/textures/level_1_completed.png");
    pLvlsTex[1] = new TextureObject("resources/textures/level_2_completed.png");

    // Caricamento modelli blender
    pCageModel = new Model("resources/models/Cage.obj");

    // Caricamento oggetti
    pLvls[0] = new GameObject(glm::vec2(SCR_WIDTH_F * 0.5f, SCR_HEIGHT_F * 0.85f), pLvlsTex[0]->getSize() * 1.06f, pLvlsTex[0], 0);
    pLvls[1] = new GameObject(glm::vec2(SCR_WIDTH_F * 0.5f, SCR_HEIGHT_F * 0.85f), pLvlsTex[1]->getSize() * 1.06f, pLvlsTex[1], 0);
    pCandy = new GameObject(glm::vec2(0.0f, 0.0f), glm::vec3(0.07f * SCR_WIDTH_F / 2.0f, 0.07f * SCR_HEIGHT_F / 2.0f * getAspect(Window), 0.1f), (TextureObject*) nullptr, 0);
    pSlime = new GameObject(glm::vec2(0.0f, 0.0f), glm::vec3(0.13f * CurrentGame->pTexSlime->getAspect() * SCR_WIDTH_F / 2.0f, 0.13f * SCR_HEIGHT_F / 2.0f, 0.1f), CurrentGame->pTexSlime, 0);
    pFloor = new GameObject(glm::vec2(0.0f, -0.95f), glm::vec3{ 2.0f, 0.1f, 0.2f }, CurrentGame->pTexPlatforms, 1);

    solidsGretel.emplace_back(pFloor);
    solidsHansel.emplace_back(pFloor);

    // Setup shader
    pCamera = new Camera(glm::vec3(0.0f, 0.0f, 0.5f));
    glm::mat4 view = pCamera->GetViewMatrix();
    pSpriteShader->use();
    pSpriteShader->setMat4("view", view);

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
    delete pText;

    delete pLvlsTex[0];
    delete pLvlsTex[1];

    delete pCageModel;

    delete pLvls[0];
    delete pLvls[1];
    delete pFloor;
    delete pCage;
    delete pCandy;
    delete pSlime;

    if (pGretel != nullptr)
        pGretel = nullptr;
    if (pHansel != nullptr)
        pHansel = nullptr;

    FT_Done_FreeType(ft);
}

ScoreState* ScoreState::GetInstance(StateManager* manager, GLFWwindow* window, irrklang::ISoundEngine* engine)
{
    static ScoreState Instance(manager, window, engine);
    return &Instance;
}
void ScoreState::EnterState()
{   
    // musica di sottofondo
    ost = Engine->play2D("resources/sounds/tad_the_end_piano.mp3", true, false, true);

    timer = timerDuration;

    if (CurrentGame->GetStatus() == GameStatus::End)
        return;

    pGretel = new Player(glm::vec2(0.0f, 0.0f), glm::vec3(0.12f, 0.12f * getAspect(Window) * CurrentGame->pTexGretel->getAspect(), 0.1f), CurrentGame->pTexGretel, 0, PlayerName::Gretel, 3);
    pHansel = new Player(glm::vec2(0.0f, 0.0f), glm::vec3(0.12f, 0.12f * getAspect(Window) * CurrentGame->pTexHansel->getAspect(), 0.1f), CurrentGame->pTexHansel, 0, PlayerName::Hansel, 3);
    pCage = new MovingObject(glm::vec2(0.0f, 0.0f), glm::vec3(0.15f, 0.15f, 0.15f), pCageModel, 0, glm::vec2(0.0f, 0.0f), 0);

    if (CurrentGame->GetLvl() == 1) {
        if (CurrentGame->IsMultiplayer()) {
            pGretel->Position = glm::vec2(-0.05f, -0.85f);
            pHansel->Position = glm::vec2(0.05f, -0.85f);            
        }
        else {
            pGretel->Position = glm::vec2(-0.5f, -0.85f);
            pHansel->Position = glm::vec2(0.75f, 0.76f);
            pCage->Position = glm::vec2(0.75f, 0.84f);
            pCage->isOnGround = false;
            pHansel->isOnGround = false;
            if (!PlayState::TeleportUnlocked)
                PlayState::TeleportUnlocked = DrawTeleportUnlocked = true;
        }

    }
    else if (CurrentGame->GetLvl() == 2) {
        if (CurrentGame->IsMultiplayer()) {
            pGretel->Position = glm::vec2(0.1f, -0.85f);
            pHansel->Position = glm::vec2(-0.1f, -0.85f);
            pGretel->pacman = false;
            pHansel->pacman = false;
            end = false;
        }
        else {
            pGretel->Position = glm::vec2(-0.5f, -0.85f);
            pHansel->Position = glm::vec2(0.75f, 0.76f);
            pCage->Position = glm::vec2(0.75f, 0.84f);
            pHansel->isOnGround = false;
            pCage->isOnGround = false;
            //Rotazione attorno al gancio della gabbia
            pivotCage = glm::vec2(0.0f, pCage->Size.y / 2.0f);
            pivotHansel = pCage->Position + pivotCage - pHansel->Position;
            pCage->SetRotation(4.0f, GameObject::axisZ, pivotCage, 0.25f);
            pHansel->SetRotation(4.0f, GameObject::axisZ, pivotHansel, 0.25f);
            pCage->totalSwings = pHansel->totalSwings = 2;
            //Reset parametri di sincronizzazione
            nPauses = 0;
            oscillate = true;
            HanselFree = false;
            cageFall = false;
        }

    }
}

void ScoreState::LeaveState()
{
    if (DrawTeleportUnlocked)
        DrawTeleportUnlocked = false;
    if (DrawMultiplayerUnlocked)
        DrawMultiplayerUnlocked = false;

    if (CurrentGame->GetStatus() != GameStatus::End) {
        if (pGretel != nullptr) {
             pGretel;
            pGretel = nullptr;
        }
        if (pHansel != nullptr) {
            delete pHansel;
            pHansel = nullptr;
        }
        if (pCage != nullptr) {
            delete pCage;
            pCage = nullptr;
        }
    }

    ost->stop();
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

    /*
    if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        ChangeState(MenuState::GetInstance(Manager, Window, Engine));
    }*/

    if (CurrentGame->GetLvl() == 1) {
        if (CurrentGame->IsMultiplayer()) {
            pGretel->Move(deltaTime);
            pGretel->CheckCollisionWithSolids(solidsGretel);
            ProcessInputPlayer(pGretel, GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_A, GLFW_KEY_D);
            pHansel->Move(deltaTime);
            pHansel->CheckCollisionWithSolids(solidsHansel);
            ProcessInputPlayer(pHansel, GLFW_KEY_UP, GLFW_KEY_DOWN, GLFW_KEY_LEFT, GLFW_KEY_RIGHT);
        }
        else {
            pGretel->Move(deltaTime);
            pGretel->CheckCollisionWithSolids(solidsGretel);
            ProcessInputPlayer(pGretel, GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_A, GLFW_KEY_D);
        }

    }
    else if (CurrentGame->GetLvl() == 2) {
        if (CurrentGame->IsMultiplayer()) {
            if (end)
                return;
            //piccolo delay iniziale
            if (timer <= 0.0f) {
                if (pGretel->velocity.x < pGretel->maxVelocityNPC.x)
                    pGretel->velocity.x += 0.01f;
                if (pHansel->velocity.x < pHansel->maxVelocityNPC.x)
                    pHansel->velocity.x += 0.01f;
            }
            else
                timer -= deltaTime;
            bool GretelExit = pGretel->Position.x >= xtarget;
            bool HanselExit = pHansel->Position.x >= xtarget;
            if (!GretelExit) {
                pGretel->Move(deltaTime);
                pGretel->CheckCollisionWithSolids(solidsGretel);
            }
            if (!HanselExit) {
                pHansel->Move(deltaTime);
                pHansel->CheckCollisionWithSolids(solidsHansel);
            }
            if (GretelExit && HanselExit)
                end = true;
        }
        else {
            pGretel->Move(deltaTime);
            pGretel->CheckCollisionWithSolids(solidsGretel);
            ProcessInputPlayer(pGretel, GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_A, GLFW_KEY_D);
            if (cageFall || HanselFree) {
                pHansel->Move(deltaTime);
                pHansel->CheckCollisionWithSolids(solidsHansel);
            }
            if (cageFall) {
                pCage->Move(deltaTime);
                pCage->CheckCollisionWithSolids(solidsHansel);
                if (pCage->isOnGround) {
                    Engine->play2D("resources/sounds/cage_fall.wav");
                    HanselFree = true;
                    cageFall = false;
                    if (!PlayState::MultiplayerUnlocked) {
                        PlayState::MultiplayerUnlocked = DrawMultiplayerUnlocked = true;
                    }
                }              
            }
            if (HanselFree)
                ProcessInputPlayer(pHansel, GLFW_KEY_UP, GLFW_KEY_DOWN, GLFW_KEY_LEFT, GLFW_KEY_RIGHT);
            else {
                if (oscillate) {
                    bool cageOscill = pCage->Oscillate(deltaTime);
                    bool hanselOscill = pHansel->Oscillate(deltaTime);
                    oscillate = cageOscill || hanselOscill;
                }
                else {
                    timer-=deltaTime;
                    if (timer <= 0.0f) {
                        oscillate = true;
                        timer = timerDuration;
                        nPauses++;
                    }
                }
                if (nPauses >= NPAUSE) {
                    cageFall = true;
                    oscillate = false;
                }
            }
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
void ScoreState::MouseClick(double xpos, double ypos, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) { 
        if (CurrentGame->GetLvl() < 2) {
            CurrentGame->CurrentLevel[CurrentGame->IsMultiplayer()]++;
            ChangeState(CurrentGame);
            CurrentGame->Reset();//Inizia nuovo livello
        }
        else {
            CurrentGame->Status[CurrentGame->IsMultiplayer()] = GameStatus::End;
            ChangeState(MenuState::GetInstance(Manager, Window, Engine));
        }
    }

}

void ScoreState::Render()
{
    //int fbWidth, fbHeight;
    //glfwGetFramebufferSize(Window, &fbWidth, &fbHeight);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDisable(GL_DEPTH_TEST);

    //Oggetti con coordinate pixel
    //--------------------------------------------------------------------------------
    ShaderManager::SetProjection(*pSpriteShader, Window, ProjectionType::Pixels);

    RenderStats();

    //Oggetti con coordinate NDC
    //--------------------------------------------------------------------------------
    ShaderManager::SetProjection(*pSpriteShader, Window, ProjectionType::NDC);

    pFloor->Render(*pSpriteShader);
    pGretel->Render(*pSpriteShader);
    pHansel->Render(*pSpriteShader);

    glEnable(GL_DEPTH_TEST);

    //glEnable(GL_MULTISAMPLE);

    //Oggetti 3D
    //--------------------------------------------------------------------------------
    if (!CurrentGame->IsMultiplayer() && (CurrentGame->GetLvl() == 1 || (CurrentGame->GetLvl() == 2 && !HanselFree))) {
        pCage->Render(*pEnlightenedShader);
    }

    //glDisable(GL_MULTISAMPLE);
}
void ScoreState::RenderStats() {
    glm::vec3 TextColor = { 255.0f, 255.0f, 255.0f };

    float height = SCR_HEIGHT_F * 0.75f;
    float text_width = SCR_WIDTH_F * 0.25;
    float gretel_width = SCR_WIDTH_F * 0.55f;
    float hansel_width = SCR_WIDTH_F * 0.75f;
    float y_indent = SCR_HEIGHT_F / 20.0f;

    pLvls[CurrentGame->GetLvl() - 1]->Render(*pSpriteShader);

    pText->Render(*pTextShader, "STATS", text_width, height, 1.0f, TextColor, Alignment::Left);

    height -= y_indent;
    pText->Render(*pTextShader, "Gretel", gretel_width, height, 1.0f, TextColor, Alignment::Right);
    if (CurrentGame->IsMultiplayer())
        pText->Render(*pTextShader, "Hansel", hansel_width, height, 1.0f, TextColor, Alignment::Right);

    pSlime->Position = glm::vec2(text_width + pSlime->Size.x / 2.0f, height - pSlime->Size.y / 2.0f - y_indent / 2.0f);
    pSlime->Render(*pSpriteShader);
    pText->Render(*pTextShader, std::to_string(CurrentGame->GretelKills), gretel_width, height - pSlime->Size.y - y_indent / 3.0f, 1.0f, TextColor, Alignment::Right);
    if (CurrentGame->IsMultiplayer()) {
        pText->Render(*pTextShader, std::to_string(CurrentGame->HanselKills), hansel_width, height - pSlime->Size.y - y_indent / 3.0f, 1.0f, TextColor, Alignment::Right);
    }
    height -= (pSlime->Size.y + y_indent / 2.0f);

    for (int i = 0; i < CurrentGame->pCandiesMesh.size(); i++) {
        bool draw = false;
        if (CurrentGame->GretelCandyStats[i] != 0) {
            pText->Render(*pTextShader, std::to_string(CurrentGame->GretelCandyStats[i]), gretel_width, height - pCandy->Size.y - y_indent / 3.0f, 1.0f, TextColor, Alignment::Right);
            draw = true;
        }
        if (CurrentGame->IsMultiplayer() && CurrentGame->HanselCandyStats[i] != 0) {
            pText->Render(*pTextShader, std::to_string(CurrentGame->HanselCandyStats[i]), hansel_width, height - pCandy->Size.y - y_indent / 3.0f, 1.0f, TextColor, Alignment::Right);
            draw = true;
        }
        if (draw) {
            //if (i < CurrentGame->pCandiesMesh.size() - 1)
            pCandy->Position = glm::vec2(text_width + pCandy->Size.x / 2.0f, height - pCandy->Size.y / 2.0f - y_indent / 2.0f);
            height -= (pCandy->Size.y + y_indent / 2.0f);
            pCandy->Texture = CurrentGame->pCandiesMesh[i];
            pCandy->Render(*pSpriteShader);
        }

    }

    height -= y_indent;
    std::string time = "TIME LEFT: " + std::to_string(CurrentGame->remainingTime);
    pText->Render(*pTextShader, time, text_width, height, 1.0f, TextColor, Alignment::Left);

    height -= y_indent;
    std::string score = "SCORE: " + std::to_string(CurrentGame->CurrentScore);
    pText->Render(*pTextShader, score, text_width, height, 1.0f, TextColor, Alignment::Left);

    if (DrawTeleportUnlocked) {
        height -= y_indent;
        pText->Render(*pTextShader, std::string("TELEPORT CANDY UNLOCKED"), text_width, height, 1.0f, TextColor, Alignment::Left);
    }

    if (DrawMultiplayerUnlocked) {
        height -= y_indent;
        pText->Render(*pTextShader, std::string("MULTIPLAYER UNLOCKED"), text_width, height, 1.0f, TextColor, Alignment::Left);
    }
}