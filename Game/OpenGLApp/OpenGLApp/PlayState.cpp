#include "PlayState.h"

#define TOTENEM 15
#define TOTKEYS 1

#define G_LIVES 3
#define H_LIVES 2

#define NP 1
#define SPAWN_MIN_E 3
#define SPAWN_MAX_E 6

#define SPAWN_PROB_C 100 //percentuale di spawn delle caramelle (bonus e malus)

#define SPAWN_PROB_SUPER 50 //percentuale di spawn delle SuperSlimes

#define FLAT 1

bool PlayState::TeleportUnlocked = false;

bool PlayState::MultiplayerUnlocked = false;
bool Multiplayer = false;

static glm::vec2 posSpawn[2] = { {-0.8f, 0.80f}, {0.8f, 0.80f} };
static glm::vec2 posSpawn2[3] = { {-0.8f, 0.06f}, {0.2f, 0.06f}, {0.8f, -0.34f} };
static glm::vec2 velocity = { 0.3f, 0.0f };
static glm::vec2 velocities[2] = { velocity, -velocity };
static glm::vec2 velocities2[3] = { velocity, -velocity, -velocity };

//std::queue<std::string> q;
std::deque<std::tuple<std::string, float>> q;
/*
static glm::vec2 positions[8] = {
   {0.0f, -0.95f},//pavimento
   {-0.6f, -0.5f}, {0.6f, -0.5f},
   {-0.85f, -0.1f}, {0.85f, -0.1f},
   {0.0f, 0.0f},
   {-0.6f, 0.5f}, {0.6f, 0.5f}
};
static glm::vec3 sizes[8] = {
    {2.0f, 0.1f, 0.2f},
    {0.8f, 0.1f, 0.2f}, {0.8f, 0.1f, 0.2f},
    {0.3f, 0.1f, 0.2f}, {0.3f, 0.1f, 0.2f},
    {1.0f, 0.1f, 0.2f},
    {0.8f, 0.1f, 0.2f}, {0.8f, 0.1f, 0.2f}
};
static glm::vec2 positions2[6] = {
   {0.0f, -0.95f},//pavimento
   {-0.2f, -0.5f}, {0.8f, -0.5f},
   {-0.8f, -0.1f}, {0.2f, -0.1f},
   {0.0f, 0.5f}//soffitto
};
static glm::vec3 sizes2[6] = {
    {2.0f, 0.1f, 0.2f},
    {0.4f, 0.1f, 0.2f}, {0.4f, 0.1f, 0.2f},
    {0.4f, 0.1f, 0.2f}, {0.4f, 0.1f, 0.2f},
    {2.0f, 0.1f, 0.2f}
};
static glm::vec2 posCauldron[2] = { {0.89f, 0.64f}, {-0.89f, 0.64f} };
static glm::vec2 posCauldron2[3] = { {-0.89f, 0.04f}, {0.29f, 0.04f}, {0.89f, -0.36f} };
static glm::vec3 sizeCauldron = { 0.1f, 0.1f, 0.1f };*/

PlayState::PlayState(StateManager* manager, GLFWwindow* window, irrklang::ISoundEngine* engine)
    : GameState(manager, window, engine), lastFrame(0.0f), deltaTime(0.0f), pGretel(nullptr), pHansel(nullptr)
{
    //int fbWidth, fbHeight;
    //glfwGetFramebufferSize(Window, &fbWidth, &fbHeight);

    pCamera = new Camera(glm::vec3(0.0f, 0.0f, 0.5f));

    // Initializza FreeType
    if (FT_Init_FreeType(&ft)) {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }

    /*
    for (int i = 0; i < 8; i++) {
        float x_ndc, y_ndc, x_pixel, y_pixel;
        x_ndc = positions[i].x;
        y_ndc = positions[i].y;
        x_pixel = (x_ndc + 1.0f) * 0.5f * fbWidth;
        y_pixel = fbHeight - ((y_ndc + 1.0f) * 0.5f * fbHeight);
        positionsTest[i] = { x_pixel, y_pixel };
        x_pixel = sizes[i].x * (fbWidth / 2.0f);
        y_pixel = sizes[i].y * (fbHeight / 2.0f);
        sizesTest[i] = { x_pixel, y_pixel, 0.1f };

        //positionsTest[i] = { x_ndc, y_ndc };
    }*/

    CurrentLevel[0] = CurrentLevel[1] = StartLevel;
    Status[0] = Status[1] = GameStatus::None;

    pTexPlatforms = new TextureObject("resources/textures/donut_block.jpg");
    //pTexPlayer = new TextureObject("resources/textures/ice_cream_block.jpg");
    pTexGretel = new TextureObject("resources/textures/gretel.png");
    pTexHansel = new TextureObject("resources/textures/hansel.png");
    //pTexEnemy = new TextureObject("resources/textures/awesomeface.png");
    pTexSlime = new TextureObject("resources/textures/slime2-mod.png");
    //pTexBackground = new TextureObject("resources/textures/dark_wood_background2.png");
    pTexSuperSlime = new TextureObject("resources/textures/superslime.png");

    pText = new TextObject(ft, "resources/fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf");

    // caricamento modelli blender
    //pCubeModel = new Model("resources/models/cubetto.obj");
    pCauldronModel = new Model("resources/models/cauldron.obj");
    //pSlimeModel = new Model("resources/models/slime2.obj");
    //pBackgroundModel = new Model("resources/models/background.obj");

    //caricamento modelli 2d
    pHeartsTex[0] = new TextureObject("resources/textures/black_heart.png");
    pHeartsTex[1] = new TextureObject("resources/textures/golden_heart.png");
    pKeysTex[0] = new TextureObject("resources/textures/key1.png");
    pKeysTex[1] = new TextureObject("resources/textures/key2.png");
    pCandiesMesh.emplace_back(new TextureObject("resources/textures/candy_pink.png"));
    pCandiesMesh.emplace_back(new TextureObject("resources/textures/candy_green.png"));
    pCandiesMesh.emplace_back(new TextureObject("resources/textures/candy_blue.png"));
    pCandiesMesh.emplace_back(new TextureObject("resources/textures/candy_orange.png"));
    pCandiesMesh.emplace_back(new TextureObject("resources/textures/candy_violet.png"));

    //Creazione oggetti UI
    pHearts[0] = new GameObject(glm::vec2(SCR_WIDTH_F * 0.05f, 895.0f), pHeartsTex[0]->getSize() / 9.0f, pHeartsTex[0], 0);
    pHearts[1] = new GameObject(glm::vec2(SCR_WIDTH_F * 0.2f, 895.0f), pHeartsTex[1]->getSize() / 9.0f, pHeartsTex[1], 0);
    pHearts[0]->Position.x -= (pHearts[0]->Size.x/2.0f);
    pHearts[1]->Position.x -= (pHearts[1]->Size.x/2.0f);
    //pHearts[1] = new GameObject(glm::vec2(fbWidth * 0.1f, pHearts[0]->Position.y - pHearts[0]->Size.y - 10.0f), pHeartsTex[1]->getSize() / 8.0f, pHeartsTex[1], 0);

    pSlimeUI = new GameObject(glm::vec2(0.0f, 0.0f), glm::vec3(0.09f * pTexSlime->getAspect() * SCR_WIDTH_F / 2.0f, 0.09f * SCR_HEIGHT_F / 2.0f, 0.1f), pTexSlime, 0);
    pKeysUI[0] = new GameObject(glm::vec2(0.0f, 0.0f), glm::vec3(0.045f * pKeysTex[0]->getAspect() * SCR_WIDTH_F / 2.0f, 0.045f * getAspect(Window) * SCR_HEIGHT_F / 2.0f, 0.1f), pKeysTex[0], 0);
    pKeysUI[1] = new GameObject(glm::vec2(0.0f, 0.0f), glm::vec3(0.045f * pKeysTex[1]->getAspect() * SCR_WIDTH_F / 2.0f, 0.045f * getAspect(Window) * SCR_HEIGHT_F / 2.0f, 0.1f), pKeysTex[1], 0);

    candySize = glm::vec3(0.07f, 0.07f * getAspect(Window), 0.1f);

    pCandyTypes.emplace_back(new CandyType(EffectType::NoJump, 5.0f));
    pCandyTypes.emplace_back(new CandyType(EffectType::Speed, 1.5f, 10.0f));
    pCandyTypes.emplace_back(new CandyType(EffectType::SpeedEnemy, 1.5f, 10.0f));
    pCandyTypes.emplace_back(new CandyType(EffectType::Invincibility, 5.0f));
    pCandyTypes.emplace_back(new CandyType(EffectType::Teleport));

    //Associazione run-time tra texture ed effetto della caramella
    std::shuffle(pCandiesMesh.begin(), pCandiesMesh.end(), rd);

    #if DEBUG
    printf("TAB CARAMELLE\n");
    for (int i = 0; i < std::min(pCandiesMesh.size(), pCandyTypes.size()); i++)
        printf("%s\t%s\n", pCandyTypes[i]->Print(), pCandiesMesh[i]->Path);
    #endif
 
    /*
    for (int i = 0; i < std::min(pCandiesMesh.size(), pCandyTypes.size()); i++) {
        typeToTextureMap.emplace(pCandyTypes[i], pCandiesMesh[i]);
    }
   
    for (auto& it : typeToTextureMap) {
        CandyType* type = it.first;
        TextureObject* texture = it.second;
        printf("%s %d", texture->Path, type->effect);
    }
    printf("\n");*/

    //pBackground = new GameObject(glm::vec2(0.0f, 0.0f), glm::vec3(1.5f, 1.5f, 1.5f), pTexBackground, 0);

    glm::mat4 projection2 = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH_F), 0.0f, static_cast<float>(SCR_HEIGHT_F));//left, right, bottom, top

    float left = -1.0f;   // Puoi modificare questi valori per adattarli alla tua scena
    float right = 1.0f;
    float bottom = -1.0f;
    float top = 1.0f;

    glm::mat4 projectionNDC = glm::ortho(left, right, bottom, top);
    glm::mat4 projectionPixels = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH_F), 0.0f, static_cast<float>(SCR_HEIGHT_F));//left, right, bottom, top

    glm::mat4 view = pCamera->GetViewMatrix();

    // setup delle uniform delle shader che non cambieranno nel ciclo di rendering
    // Shader base
    pShader->use();
    pShader->setMat4("projection", projectionPixels);
    pShader->setMat4("view", view);

    // Shader sprite
    pSpriteShader->use();
    pSpriteShader->setMat4("projection", projectionNDC);
    pSpriteShader->setMat4("view", view);

    //Shader per materiale metallico
    pEnlightenedShader->use();
    pEnlightenedShader->setMat4("projection", projectionNDC);
    pEnlightenedShader->setMat4("view", view);

    pEnlightenedShader->setVec3("viewPos", pCamera->Position);

    
    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
    glm::vec3 lightPosition(0.0f, -0.0f, 2.0f);
    //glm::vec3 lightPosition(0.0f, -0.75f, 1.2f);

   
    ShaderManager::SetMaterial(*pEnlightenedShader, ShaderManager::Silver);
    ShaderManager::SetLight(*pEnlightenedShader, lightPosition, lightColor);
   
    /*
    // valori per materiale dorato
    pEnlightenedShader->setVec3("material.ambient", glm::vec3(0.24725f, 0.1995f, 0.0745f));
    pEnlightenedShader->setVec3("material.diffuse", glm::vec3(0.75164f, 0.60648f, 0.22648f));
    pEnlightenedShader->setVec3("material.specular", glm::vec3(0.628281f, 0.555802f, 0.366065f));
    pEnlightenedShader->setFloat("material.shininess", 128.0f * 0.4f);

    // posizione fonte di luce
    pEnlightenedShader->setVec3("light.position", lightPosition);

    // parametri luce
    pEnlightenedShader->setVec3("light.ambient", lightColor * glm::vec3(0.3f));
    pEnlightenedShader->setVec3("light.diffuse", lightColor * glm::vec3(0.6f));
    pEnlightenedShader->setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f)); */

    // cattura il tempo iniziale di gioco
    startTime = glfwGetTime();
}
PlayState::~PlayState()
{
    //public:
    delete pTexGretel;
    delete pTexHansel;
    delete pTexPlatforms;
    delete pTexSlime;

    for (TextureObject* p : pCandiesMesh)
        delete p;
    for (CandyType* p : pCandyTypes)
        delete p;

    //private:
    delete pText;
    delete pCubeModel;
    delete pCauldronModel;
    delete pSlimeModel;
    delete pSlimeUI;

    if (pGretel != nullptr)
        delete pGretel;
    if (pHansel != nullptr)
        delete pHansel;

    for (int i = 0; i < 2; i++) {
        pKeysTex[i];
        pHeartsTex[i];
        pHearts[i];
        pKeysUI[i];
        pKeys[i];
    }

    for (Enemy* p : pEnemies)
        delete p;
    for (Candy* p : pCandies)
        delete p;
    for (GameObject* p : platforms)
        delete p;
    for (GameObject* p : pCauldrons)
        delete p;

    //Distrugge FreeType
    FT_Done_FreeType(ft);
}
PlayState* PlayState::GetInstance(StateManager* manager, GLFWwindow* window, irrklang::ISoundEngine* engine)
{
    static PlayState Instance(manager, window, engine);
    return &Instance;
}

void PlayState::Reset()
{
    if (CurrentLevel[Multiplayer] > 2)
        return;

    glm::vec2 positions[8] = {
       {0.0f, -0.95f},//pavimento
       {-0.6f, -0.5f}, {0.6f, -0.5f},
       {-0.85f, -0.1f}, {0.85f, -0.1f},
       {0.0f, 0.0f},
       {-0.6f, 0.5f}, {0.6f, 0.5f}
    };
    glm::vec3 sizes[8] = {
        {2.0f, 0.1f, 0.2f},
        {0.8f, 0.1f, 0.2f}, {0.8f, 0.1f, 0.2f},
        {0.3f, 0.1f, 0.2f}, {0.3f, 0.1f, 0.2f},
        {1.0f, 0.1f, 0.2f},
        {0.8f, 0.1f, 0.2f}, {0.8f, 0.1f, 0.2f}
    };
    glm::vec2 positions2[6] = {
       {0.0f, -0.95f},//pavimento
       {-0.2f, -0.5f}, {0.8f, -0.5f},
       {-0.8f, -0.1f}, {0.2f, -0.1f},
       {0.0f, 0.5f}//soffitto
    };
    glm::vec3 sizes2[6] = {
        {2.0f, 0.1f, 0.2f},
        {0.4f, 0.1f, 0.2f}, {0.4f, 0.1f, 0.2f},
        {0.4f, 0.1f, 0.2f}, {0.4f, 0.1f, 0.2f},
        {2.0f, 0.1f, 0.2f}
    };
    glm::vec2 posCauldron[2] = { {0.89f, 0.64f}, {-0.89f, 0.64f} };
    glm::vec2 posCauldron2[3] = { {-0.89f, 0.04f}, {0.29f, 0.04f}, {0.89f, -0.36f} };
    glm::vec3 sizeCauldron = { 0.1f, 0.1f, 0.1f };
    
    CurrentScore = 0;

    if (pGretel != nullptr) {
        delete pGretel;
        pGretel = nullptr;
    }      
    if (pHansel != nullptr) {
        delete pHansel;
        pHansel = nullptr;
    }
    if (!pCauldrons.empty()) {
        for (GameObject* pc : pCauldrons)
            delete pc;
        pCauldrons.clear();
    }    
    if (!platforms.empty()) {
        for (GameObject* pp : platforms)
            delete pp;
        platforms.clear();
    }
    if (!pEnemies.empty()) {
        for (Enemy* pe : pEnemies)
            delete pe;
        pEnemies.clear();
    }
    nEnemiesKilled = 0;
    nEnemiesAlive = 0;

    if (!pCandies.empty()) {
        for (Candy* pc : pCandies)
            delete pc;
        pCandies.clear();
    }
    pCandyTypes.emplace_back(new CandyType(EffectType::NoJump, 10.0f));
    pCandyTypes.emplace_back(new CandyType(EffectType::Speed, 1.5f, 10.0f));
    pCandyTypes.emplace_back(new CandyType(EffectType::SpeedEnemy, 1.5f, 10.0f));
    pCandyTypes.emplace_back(new CandyType(EffectType::Invincibility, 5.0f));
    pCandyTypes.emplace_back(new CandyType(EffectType::Teleport));
    
    if (!pProbabilities.empty())
        pProbabilities.clear();
    if (TeleportUnlocked == false) {
        //NoJump, Speed, SpeedEnemy, Invincibility, Teleport
/*
        pProbabilities.emplace_back(25);
        pProbabilities.emplace_back(25);
        pProbabilities.emplace_back(25);
        pProbabilities.emplace_back(25);
        pProbabilities.emplace_back(0);
        */
        pProbabilities.emplace_back(0);
        pProbabilities.emplace_back(0);
        pProbabilities.emplace_back(0);
        pProbabilities.emplace_back(0);
        pProbabilities.emplace_back(100);
        
    }
    else {
        /*
        pProbabilities.emplace_back(21);
        pProbabilities.emplace_back(21);
        pProbabilities.emplace_back(21);
        pProbabilities.emplace_back(21);
        pProbabilities.emplace_back(16);// Teleport è più rara
        */
        pProbabilities.emplace_back(0);
        pProbabilities.emplace_back(0);
        pProbabilities.emplace_back(0);
        pProbabilities.emplace_back(0);
        pProbabilities.emplace_back(100);
        
    }

    /*
    pGretel = new Player(glm::vec2(0.0f, -0.75f), glm::vec3(0.1f, 0.1f, 0.1f), pCubeModel, pTexPlayer, 0, PlayerName::Gretel);
    if(Multiplayer)
        pHansel = new Player(glm::vec2(0.0f, -0.75f), glm::vec3(0.1f, 0.1f, 0.1f), pCubeModel, pTexPlayer, 0, PlayerName::Hansel);
    */

    if(Multiplayer == false)
        pGretel = new Player(glm::vec2(0.0f, -0.85f), glm::vec3(0.12f, 0.12f * getAspect(Window) * pTexGretel->getAspect(), 0.1f), pTexGretel, 0, PlayerName::Gretel, G_LIVES);
    else {
        pGretel = new Player(glm::vec2(-0.05f, -0.85f), glm::vec3(0.12f, 0.12f * getAspect(Window) * pTexGretel->getAspect(), 0.1f), pTexGretel, 0, PlayerName::Gretel, H_LIVES);
        pHansel = new Player(glm::vec2(0.05f, -0.85f), glm::vec3(0.12f, 0.12f * getAspect(Window) * pTexHansel->getAspect(), 0.1f), pTexHansel, 0, PlayerName::Hansel, H_LIVES);
    }

    nKeys = 0;

    if (CurrentLevel[Multiplayer] == 1) {
        // passo nullptr come texture per ora
        for (int i = 0; i < 2; i++)
        {
            pCauldrons.emplace_back(new GameObject(posCauldron[i], sizeCauldron, pCauldronModel, 0));
        }

        for (int i = 0; i < 8; ++i)
        {
#if FLAT
            platforms.emplace_back(new GameObject(positions[i], sizes[i], pTexPlatforms, 1));
#else
            platforms.emplace_back(new GameObject(positions[i], sizes[i], pCubeModel, 1));
#endif
        }
        /*
        float l = fbWidth / 20;
        tests.emplace_back(glm::vec2(fbWidth/ 2, l/2), glm::vec3(l*20.0f, l, 0.0f), pTest, 1);
        tests.emplace_back(glm::vec2(fbWidth/5, 3*l + l/2), glm::vec3(l * 8.0f, l, 0.0f), pTest, 1);
        tests.emplace_back(glm::vec2(fbWidth*4/5, 3*l +l/2), glm::vec3(l * 8.0f, l, 0.0f), pTest, 1);
        tests.emplace_back(glm::vec2(fbWidth / 5, 3 * l + l / 2), glm::vec3(l * 3.0f, l, 0.0f), pTest, 1);
        tests.emplace_back(glm::vec2(fbWidth * 4 / 5, 3 * l + l / 2), glm::vec3(l * 3.0f, l, 0.0f), pTest, 1);
        */
    }
    else if (CurrentLevel[Multiplayer] == 2) {
        for (int i = 0; i < 3; i++)
        {
            pCauldrons.emplace_back(new GameObject(posCauldron2[i], sizeCauldron, pCauldronModel, 0));
        }
        for (int i = 0; i < 6; ++i)
        {
#if FLAT
            platforms.emplace_back(new GameObject(positions2[i], sizes2[i], pTexPlatforms, 1));
#else
            platforms.emplace_back(new GameObject(positions2[i], sizes2[i], pCubeModel, 1));
#endif
        }

        if (pKeys[Multiplayer] == nullptr)
            pKeys[Multiplayer] = new GameObject(glm::vec2(0.0f, 0.61f),
                                                glm::vec3(0.09f * pKeysTex[Multiplayer]->getAspect(), 0.09f * getAspect(Window), 0.1f),
                                                pKeysTex[Multiplayer], 0);
    }

    //Svuota coda effetti
    q.clear();

    lastSpawnTime = 0.0f;
    spawnTime = 0;
    spawnPlace = RandomInt(0, pCauldrons.size() - 1);
    #if DEBUG
    printf("Prossimo spawn tra %d s nel calderone %d\n", spawnTime, spawnPlace);
    #endif

    if (Status[Multiplayer] != GameStatus::Paused) {
        currentTime = 0;
        totalPauseTime = 0.0f;
        startPauseTime = 0.0f;
        startTime = glfwGetTime();
    }
}
//DEBOUNCE: avoids that pressing key in other frames (or states) is considered in this frame (or state)
bool canPressDOWN = true;
void PlayState::ProcessInput()
{
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;

    if (deltaTime > 0.1f)// Ignora delta troppo grandi
        deltaTime = 0.0f;

    lastFrame = currentFrame;

    if (isEnding) {
        if (!pGretel->isDead) {
            pGretel->Move(deltaTime);
            pGretel->CheckCollisionWithSolids(platforms);
            pGretel->Update(deltaTime);

        }
        if (Multiplayer && !pHansel->isDead) {
            pHansel->Move(deltaTime);
            pHansel->CheckCollisionWithSolids(platforms);
            pHansel->Update(deltaTime);
        }
        return;
    }

    // debug Gretel
    if (glfwGetKey(Window, GLFW_KEY_C) == GLFW_PRESS)
    {
        #if DEBUG
        std::cout << ((pGretel->isOnGround == true) ? "A terra" : "In aria") << std::endl;
        std::cout << "Velocity: " << pGretel->velocity.x << ", " << pGretel->velocity.y << std::endl;
        std::cout << "Bottom-Left: " << pGretel->Position.x - (pGretel->Size.x / 2) << ", " << pGretel->Position.y - (pGretel->Size.y / 2) << std::endl;
        std::cout << "HB-Bottom-Left: " << pGretel->GetHitbox().Min.x << ", " << pGretel->GetHitbox().Min.y << std::endl;
        std::cout << "HB-Top-Right: " << pGretel->GetHitbox().Max.x << ", " << pGretel->GetHitbox().Max.y << std::endl;
        std::cout << "Position-X: " << pGretel->Position.x << std::endl;
        #endif
    }

    //debug skip livelli
    if (glfwGetKey(Window, GLFW_KEY_V) == GLFW_PRESS)
    {
        Status[Multiplayer] = GameStatus::Victory;
    }

    //debug morte
    if (glfwGetKey(Window, GLFW_KEY_L) == GLFW_PRESS)
    {
        Status[Multiplayer] = GameStatus::GameOver;
    }

    if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        Status[Multiplayer] = GameStatus::Paused;
        ChangeState(MenuState::GetInstance(Manager, Window, Engine));
        return;
    }

    if (!pGretel->isDead) {
        ProcessInputPlayer(pGretel, GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_A, GLFW_KEY_D);
        pGretel->Move(deltaTime);
        pGretel->CheckCollisionWithSolids(platforms);
        if (nKeys < TOTKEYS && CurrentLevel[Multiplayer] == 2 && pGretel->CheckCollision(pKeys[Multiplayer]) != MovingObject::Collision::None) {
            Engine->play2D("resources/sounds/key_pickup.wav");
            nKeys++;
            if(nKeys == TOTKEYS)
                Status[Multiplayer] = GameStatus::Victory;
        }
        pGretel->Update(deltaTime);

    }
    if (Multiplayer && !pHansel->isDead) {
        ProcessInputPlayer(pHansel, GLFW_KEY_UP, GLFW_KEY_DOWN, GLFW_KEY_LEFT, GLFW_KEY_RIGHT);
        pHansel->Move(deltaTime);
        pHansel->CheckCollisionWithSolids(platforms);
        if (nKeys < TOTKEYS && CurrentLevel[Multiplayer] == 2 && nKeys > 0 && pHansel->CheckCollision(pKeys[Multiplayer]) != MovingObject::Collision::None) {
            Engine->play2D("resources/sounds/key_pickup.wav");
            nKeys++;
            if (nKeys == TOTKEYS)
                Status[Multiplayer] = GameStatus::Victory;
        }
        pHansel->Update(deltaTime);
    }

}
void PlayState::ProcessInputPlayer(Player* pPlayer, unsigned int UP, unsigned int DOWN, unsigned int LEFT, unsigned int RIGHT)
{
    if (pPlayer->teleport)
        return;

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

    // raccolta caramelle
    if (glfwGetKey(Window, DOWN) == GLFW_PRESS && canPressDOWN)
    {
        Candy* pCloser = pPlayer->CheckCollisionWithCandies(pCandies);

        if (pCloser != nullptr) {
            std::string output = pPlayer->EatCandy(pCloser->GetType(), Engine);
            if (q.empty())
                newDisplay = true;
            if(Player::teleport)
                q.push_back(std::make_tuple(output, displayDuration));
            else {
                if (q.empty()) q.push_back(std::make_tuple(output, displayDuration));
                else q.push_back(std::make_tuple(output, displayDurationNonEmpty));
            }
            EffectType effect = pCloser->GetEffect();
            //Add score
            if(effect == EffectType::NoJump || effect == EffectType::SpeedEnemy)
                CurrentScore += scoreMalus;
            else if (effect == EffectType::Speed || effect == EffectType::Invincibility)
                CurrentScore += scoreBonus;
            else if (effect == EffectType::Teleport)
                CurrentScore += scoreTelep;
            //printf("Player ha mangiato una caramella misteriosa\n");
        }

        canPressDOWN = false;
    }
    else if (glfwGetKey(Window, DOWN) == GLFW_RELEASE)
        canPressDOWN = true;
}
void PlayState::ProcessEvents()
{
    if (isEnding) {
        for (Enemy* pe : pEnemies) {
            if (!pe->IsDead()) {
                pe->Move(deltaTime);
                pe->CheckCollisionWithSolids(platforms);
            }
        }
        for (Candy* pc : pCandies) {
            if (!pc->IsEaten()) {
                pc->CheckCollisionWithSolids(platforms);
                pc->Move(deltaTime);
            }
        }
        CheckEndGame();
        return;
    }

    if (pEnemies.size() < TOTENEM) {
        lastSpawnTime += deltaTime;

        if (lastSpawnTime >= spawnTime) {
            Enemy* enemy;
            if (CurrentLevel[Multiplayer] == 1) {
                if (DoAction(SPAWN_PROB_SUPER)) {
                    enemy = new Enemy(posSpawn[spawnPlace], glm::vec3(0.13f * pTexSuperSlime->getAspect(), 0.13f, 0.1f), pTexSuperSlime, 0, velocities[spawnPlace], true, EnemyType::SuperSlime);
                }
                else {
                    enemy = new Enemy(posSpawn[spawnPlace], glm::vec3(0.13f * pTexSlime->getAspect(), 0.13f, 0.1f), pTexSlime, 0, velocities[spawnPlace], true, EnemyType::Slime);
                }
            }
            else if (CurrentLevel[Multiplayer] == 2) {
                if (DoAction(SPAWN_PROB_SUPER)) {
                    enemy = new Enemy(posSpawn2[spawnPlace], glm::vec3(0.13f * pTexSuperSlime->getAspect(), 0.13f, 0.1f), pTexSuperSlime, 0, velocities2[spawnPlace], true, EnemyType::SuperSlime);
                }
                else {
                    enemy = new Enemy(posSpawn2[spawnPlace], glm::vec3(0.13f * pTexSlime->getAspect(), 0.13f, 0.1f), pTexSlime, 0, velocities2[spawnPlace], true, EnemyType::Slime);
                }
            }
            pEnemies.emplace_back(enemy);
            nEnemiesAlive++;
            lastSpawnTime = 0.0f;
            spawnTime = RandomInt(SPAWN_MIN_E, SPAWN_MAX_E);
            spawnPlace = RandomInt(0, pCauldrons.size() - 1);
            Engine->play2D("resources/sounds/bubbles.wav");
            #if DEBUG
            printf("Prossimo spawn tra %d s nel calderone %d\n", spawnTime, spawnPlace);
            #endif
        }
    }

    for (Enemy* pe : pEnemies) {
        if (pe->IsDead())
            continue;
        if (pGretel->CheckEnemyCollision(pe, Engine) || (Multiplayer && pHansel->CheckEnemyCollision(pe, Engine))) {
            if (pGretel->isDead || (Multiplayer && pHansel->isDead)) {//Se un player muore
                Status[Multiplayer] = GameStatus::GameOver;
                break;
            }
            if (pe->IsDead()) {//Se enemy muore
                nEnemiesAlive--;
                nEnemiesKilled++;
                CurrentScore += scoreEnemy;
                //printf("Nemico morto a %f, %f\n", pe->Position.x, pe->Position.y);
                if (CurrentLevel[Multiplayer] == 1 && nEnemiesKilled == TOTENEM) {
                    Status[Multiplayer] = GameStatus::Victory;
                }
                //spawn candy
                if (DoAction(SPAWN_PROB_C)) {
                    //Coordinate pixel
                    //float pixelX = (pe->Position.x + 1.0f) * 0.5f * fbWidth;
                    //float pixelY = (pe->Position.y + 1.0f) * 0.5f * fbHeight;
                    //float scale = 0.05f;
                    /*
                    pixelY-=pe->Size.y;
                    pCandies.emplace_back(new GameObject(glm::vec2(pixelX, pixelY+pCandiesMesh[0]->getHeigth()*scale/2), pCandiesMesh[0]->getSize()*scale, pCandiesMesh[0], 0));
                    */
                    //pCandies.emplace_back(new Candy(glm::vec2(pixelX, pixelY), pCandiesMesh[0]->getSize() * scale, pCandiesMesh[0], 0));
                    //Coordinate NDC
                    float pixelX = pe->Position.x;
                    float pixelY = pe->Position.y;
                    //Estrazione della pillola con probabilità pesate di pProbabilities
                    std::discrete_distribution<> distCandies(pProbabilities.begin(), pProbabilities.end());
                    int rdindex = distCandies(gen);
                    CandyType* type = pCandyTypes[rdindex];
                    TextureObject* texture = pCandiesMesh[rdindex];
                    pCandies.emplace_back(new Candy(glm::vec2(pixelX, pixelY), candySize, texture, 0, *type));
#if DEBUG
                    printf("Spawnata caramella di tipo %s\n", type->Print());
#endif
                }
                continue;
            }
            else if (pe->getLives()==1 && pe->type == EnemyType::SuperSlime){
                pe->Texture = pTexSlime;
                pe->type = EnemyType::Slime;
            }
        }
        pe->Move(deltaTime);  // Aggiorna la posizione del nemico con controllo delle collisioni
        pe->CheckCollisionWithSolids(platforms);
    }

    for (Candy* pc : pCandies) {
        if (!pc->IsEaten()) {
            pc->CheckCollisionWithSolids(platforms);
            pc->Move(deltaTime);
        }
    }

    CheckEndGame();
}
void PlayState::CheckEndGame() {
    if (!isEnding){
        if (Status[Multiplayer] == GameStatus::GameOver || Status[Multiplayer] == GameStatus::Victory) {
            #if DEBUG
            printf("End game started\n");
            #endif
            pGretel->Stop(deltaTime);
            if(Multiplayer)
                pHansel->Stop(deltaTime);
            CurrentScore += remainingTime * scoreTime;
            isEnding = true;
        }
        return;
    }

    // Aggiorna il timer fine gioco
    endingTimer += deltaTime;
    if (endingTimer < endingDuration)
        return;
    #if DEBUG
    printf("Ending...\n");
    #endif
    endingTimer = 0.0f;
    isEnding = false;

    if (Status[Multiplayer] == GameStatus::GameOver) {
        //reset !
        ChangeState(EndState::GetInstance(Manager, Window, Engine));
    }
    else if (Status[Multiplayer] == GameStatus::Victory) {
        //Aggiorna statistiche di fine partita
        pGretel->GetStats(pCandyTypes, GretelCandyStats, GretelKills);
        if (Multiplayer)
            pHansel->GetStats(pCandyTypes, HanselCandyStats, HanselKills);
        ChangeState(ScoreState::GetInstance(Manager, Window, Engine));
    }

}
void PlayState::UpdateTime(long currentTime)
{

}

void PlayState::Render()
{
    //Aggiunte le seguenti due righe per gestire correttamente la trasparenza
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDisable(GL_DEPTH_TEST);

    //Oggetti con coordinate NDC
    //--------------------------------------------------------------------------------

    ShaderManager::SetProjection(*pSpriteShader, Window, ProjectionType::NDC);

    //pBackground->Render(*pShader);
    /*
    for (const GameObject& object : tests)
    {
        object.RenderFlat(*pSpriteShader);
    }*/

    for (const GameObject* pp : platforms)
#if FLAT
        pp->Render(*pSpriteShader);
#else
        pp->Render(*pShader);
#endif

    // disattiva depth buffer quando si disegnano oggetti trasparenti (interferisce con blending)
    //glDepthMask(GL_FALSE);

    if (CurrentLevel[Multiplayer] == 2)
        pKeys[Multiplayer]->Render(*pSpriteShader);

    if (!pCandies.empty()) {
        for (Candy* pc : pCandies) {
            if (!pc->IsEaten()) {
                pc->Render(*pSpriteShader);
            }
        }
    }
  
    if (!pEnemies.empty()) {
        for (Enemy* pe : pEnemies) {
            if (!pe->IsDead()) {
  #if FLAT
                pe->Render(*pSpriteShader);
#else
                pe->Render(*pShader);
#endif
                //printf("Renderizzo nemico %d, morto? %d\n", i + 1, pEnemies[i]->IsDead());
            }
        }
    }

    if(!pGretel->isDead)
        pGretel->Render(*pSpriteShader);
    if(Multiplayer && !pHansel->isDead)
        pHansel->Render(*pSpriteShader);

    if (Player::teleport) {
        //Mouse->Move();
        Mouse->Render(*pShader);
    }

    //glDepthMask(GL_TRUE); // riattiva depth buffer
    glEnable(GL_DEPTH_TEST);

    //Oggetti 3D
    //--------------------------------------------------------------------------------

    for (GameObject* pc : pCauldrons)
        pc->Render(*pEnlightenedShader);

    RenderStats();
}
void PlayState::MouseMoving(double xpos, double ypos)
{
    if (!isEnding && Player::teleport)
        Mouse->Move(xpos, ypos);
}
void PlayState::MouseClick(double xpos, double ypos, int button, int action, int mods)
{
    if (!isEnding && Player::teleport && button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        //Controlla se stia teleportando fuori schermo
        if (xpos <= 0 || xpos > SCR_WIDTH_F || ypos <= 0 || ypos > SCR_HEIGHT_F) {
            //printf("Coordinate fuori schermo -> RIPROVARE\n");
            return;
        }
        //Trasforma in coordinate NDC
        //xpos = (xpos + 1.0f) * 0.5f * fbWidth;
        //ypos = (ypos + 1.0f) * 0.5f * fbHeight;
        xpos = 2*xpos / SCR_WIDTH_F - 1.0f;
        ypos = 2*ypos / SCR_HEIGHT_F - 1.0f;
        //Controlla se stia teleportando in area vietata
        for (GameObject* pp : platforms) {
            Hitbox bounds = pp->GetHitbox();
            bool isColliding = (xpos <= bounds.Max.x && xpos >= bounds.Min.x && ypos <= bounds.Max.y && ypos >= bounds.Min.y);
            if (isColliding) {
                //printf("Coordinate collidono con ostacoli -> RIPROVARE\n");
                return;
            }
        }
        //Se le coordinate sono corrette teleporta
        float xoffset = 0.05f;
        //xoffset = (xoffset / SCR_WIDTH_F) * 2.0f;
        
        if(Multiplayer){
            pGretel->Teleport(glm::vec2(xpos - xoffset, ypos), Engine);
            pHansel->Teleport(glm::vec2(xpos + xoffset, ypos), Engine);
        }
        else {
            pGretel->Teleport(glm::vec2(xpos, ypos), Engine);
        }
        //printf("Teleport effettuato\n");
    }
}
void PlayState::EnterState()
{
    //End of the game
    if (CurrentLevel[Multiplayer] == 3) {
        ChangeState(ScoreState::GetInstance(Manager, Window, Engine));
        return;
    }
        
    Mouse->Hide();

    // musica di sottofondo
    ost = Engine->play2D("resources/sounds/kim_lightyear_angel_eyes_piano_version.wav", true, false, true);

    if (Status[Multiplayer] == GameStatus::Paused)//salta prima chiamata
        totalPauseTime += glfwGetTime() - startPauseTime;

    //Aggiorna lo stato delle modalità di gioco
    Status[Multiplayer] = GameStatus::Playing;
    if(Status[!Multiplayer] != GameStatus::None)
        Status[!Multiplayer] = GameStatus::NotPlaying;
}

void PlayState::LeaveState() {
    Mouse->Show();

    ost->stop();

    if(Status[Multiplayer] == GameStatus::Paused)
        startPauseTime = glfwGetTime();
}

void PlayState::RenderStats() {
    //int fbWidth, fbHeight;
    //glfwGetFramebufferSize(Window, &fbWidth, &fbHeight);

    if (!isEnding) {
        currentTime = static_cast<int>((glfwGetTime() - startTime) - totalPauseTime);

        remainingTime = start - currentTime;

        // se scade il tempo perdo e chiudo il gioco
        if (remainingTime <= 0)
        {
            Status[Multiplayer] = GameStatus::GameOver;
        }
    }

    //Oggetti con coordinate pixel
    //--------------------------------------------------------------------------------
    ShaderManager::SetProjection(*pSpriteShader, Window, ProjectionType::Pixels);

    float xstart = pHearts[0]->Position.x;
    //float xspace = 10.0f;
    float xspace = SCR_WIDTH_F * 0.01;

    for (int i = 0; i < pGretel->lives; i++) {
        pHearts[0]->Position.x = pHearts[0]->Position.x + pHearts[0]->Size.x + xspace;
        pHearts[0]->Render(*pSpriteShader);
    }
    pHearts[0]->Position.x = xstart;

    if (Multiplayer) {
        xstart = pHearts[1]->Position.x;
        for (int i = 0; i < pHansel->lives; i++) {
            pHearts[1]->Position.x = pHearts[1]->Position.x + pHearts[1]->Size.x + xspace;
            pHearts[1]->Render(*pSpriteShader);
        }
        pHearts[1]->Position.x = xstart;
    }

    //Testo (per ultimo per essere davanti a tutto)
    //--------------------------------------------------------------------------------

    char buf[100];
    glm::vec3 color = { 255.0, 255.0, 255.0 };

    float height = SCR_HEIGHT * 0.92f;

    /*
    std::string time = "TIME: " + std::to_string(remainingTime);
    pText->Render(*pTextShader, time, 200.0f, 1100.0f, 1.0f, glm::vec3(255.0, 255.0, 255.0));

    std::string lives = "LIVES: " + std::to_string(pPlayer->lives);
    pText->Render(*pTextShader, lives, 1200.0f, 1100.0f, 1.0f, glm::vec3(255.0, 255.0, 255.0));*/

    //std::string time = "TIME: " + std::to_string(remainingTime);
    snprintf(buf, 100, "TIME: %02d", remainingTime);
    std::string time = buf;
    //pText->Render(*pTextShader, time, 160.0f, 880.0f, 1.0f, glm::vec3(255.0, 255.0, 255.0), Alignment::Left);
    //pText->Render(*pTextShader, time, fbWidth * 0.8f, 880.0f, 0.9f, color, Alignment::Left);
    pText->Render(*pTextShader, time, SCR_WIDTH_F * 0.8f, height, 0.9f, color, Alignment::Left);

    //std::string lives = "LIVES: " + std::to_string(pGretel->lives);
    //pText->Render(*pTextShader, lives, 960.0f, 880.0f, 1.0f, glm::vec3(255.0, 255.0, 255.0), Alignment::Left);
    //pText->Render(*pTextShader, lives, fbWidth * 0.2f, 880.0f, 1.0f, glm::vec3(255.0, 255.0, 255.0), Alignment::Center);

    //std::string level = "LEVEL " + std::to_string(CurrentLevel[Multiplayer]);
    //pText->Render(*pTextShader, level, fbWidth * 0.4f, 880.0f, 1.3f, glm::vec3(255.0, 255.0, 255.0), Alignment::Left);
    //pText->Render(*pTextShader, level, fbWidth * 0.5f, 880.0f, 1.3f, color, Alignment::Center);


    //pText->Render(*pTextShader, std::string("SCORE"), fbWidth * 0.4f, 880.0f, 1.0f, color, Alignment::Center);
    snprintf(buf, 100, "%05d", CurrentScore);
    std::string score = buf;
    //pText->Render(*pTextShader, score, fbWidth * 0.5f, 840.0f, 0.9f, color, Alignment::Center);
    //pText->Render(*pTextShader, score, fbWidth * 0.4f, 880.0f, 0.9f, color, Alignment::Center);
    pText->Render(*pTextShader, score, SCR_WIDTH_F * 0.4f, height, 0.9f, color, Alignment::Center);

    if (CurrentLevel[Multiplayer] == 1) {
        pSlimeUI->Position = glm::vec2(SCR_WIDTH_F * 0.6f - pSlimeUI->Size.x / 2.0f, 895.0f);
        pSlimeUI->Render(*pSpriteShader);
        snprintf(buf, 100, "%d/%d", nEnemiesKilled, TOTENEM);
        std::string enemies = buf;
        //pText->Render(*pTextShader, enemies, pSlimeUI->Position.x + pSlimeUI->Size.x / 2.0f + 15.0f, 880.0f, 0.9f, color, Alignment::Left);
        pText->Render(*pTextShader, enemies, pSlimeUI->Position.x + pSlimeUI->Size.x / 2.0f + xspace, height, 0.9f, color, Alignment::Left);
    }
    else if (CurrentLevel[Multiplayer] >= 2) {
        pKeysUI[Multiplayer]->Position = glm::vec2(SCR_WIDTH_F * 0.6f - pKeysUI[Multiplayer]->Size.x / 2.0f, 895.0f);
        pKeysUI[Multiplayer]->Render(*pSpriteShader);
        snprintf(buf, 100, "%d/%d", nKeys, TOTKEYS);
        std::string keys = buf;
        //pText->Render(*pTextShader, keys, pKeysUI[Multiplayer]->Position.x + pKeysUI[Multiplayer]->Size.x / 2.0f + 15.0f, 880.0f, 0.9f, color, Alignment::Left);
        pText->Render(*pTextShader, keys, pKeysUI[Multiplayer]->Position.x + pKeysUI[Multiplayer]->Size.x / 2.0f + xspace, height, 0.9f, color, Alignment::Left);
    }

    //Render effetti caramelle giocatori

    if (q.empty())
        return;

    std::string& effect = std::get<0>(q[0]);
    //pText->Render(*pTextShader, effect, fbWidth * 0.5f, 820.0f, 1.0f, color, Alignment::Center);
    pText->Render(*pTextShader, effect, SCR_WIDTH_F * 0.5f, SCR_HEIGHT_F * 0.86, 1.0f, color, Alignment::Center);

    if (newDisplay) {
        //fa partire il timer dal prossimo render
        newDisplay = false;
        return;
    }
    /*
    if (Player::teleport && effect.compare("TELEPORT"))
        return;*/

    float& displayTimer = std::get<1>(q[0]);
    displayTimer -= deltaTime;
    /*
    if (q.size() > 1 && std::get<1>(q[q.size() - 1]) > displayDurationNonEmpty)
        std::get<1>(q[q.size() - 1]) -= (displayDuration - displayDurationNonEmpty);

    // Aggiorna il timer per display caramelle mangiate
    while (!q.empty() && std::get<1>(q[0]) <= 0.0f) {
        q.pop_front();
    }*/

    if (displayTimer <= 0.0f) {
        q.pop_front();
    }

}
