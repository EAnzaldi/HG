#include "PlayState.h"

#define NP 1
#define SPAWN_MIN_E 3
#define SPAWN_MAX_E 6

#define SPAWN_PROB_C 100 //percentuale di spawn delle caramelle (bonus e malus)

#define FLAT 1

bool PlayState::Multiplayer = false;
bool PlayState::MultiplayerUnlocked = true;

static glm::vec2 posSpawn[2] = { {-0.8f, 0.80f}, {0.8f, 0.80f} };
static glm::vec2 posSpawn2[3] = { {-0.8f, 0.06f}, {0.2f, 0.06f}, {0.8f, -0.34f} };
static glm::vec2 velocity = { 0.3f, 0.0f };
static glm::vec2 velocities[2] = { velocity, -velocity };
static glm::vec2 velocities2[3] = { velocity, -velocity, -velocity };
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
    : GameState(manager, window, engine), lastFrame(0.0f), deltaTime(0.0f), CurrentLevel(StartLevel)
{
    int fbWidth, fbHeight;
    glfwGetFramebufferSize(Window, &fbWidth, &fbHeight);

    pCamera = new Camera(glm::vec3(0.0f, 0.0f, 0.5f));

    // build and compile our shader zprogram
    // -------------------------------------
    pShader = new Shader("shader.vs", "shader.fs");
    pTextShader = new Shader("shader_text.vs", "shader_text.fs");
    pEnlightenedShader = new Shader("enlightened_object_shader.vs", "enlightened_object_shader.fs");
    //pEnlightenedTexturedShader = new Shader("enlightened_textured_shader.vs", "enlightened_textured_shader.fs");
    pSpriteShader = new Shader("shader_sprite.vs", "shader_sprite.fs");

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

    pTexPlatforms = new TextureObject("resources/textures/donut_block.jpg");
    //pTexPlayer = new TextureObject("resources/textures/ice_cream_block.jpg");
    pTexGretel = new TextureObject("resources/textures/gretel.png");
    pTexHansel = new TextureObject("resources/textures/hansel.png");
    pTexEnemy = new TextureObject("resources/textures/awesomeface.png");
    pTexSlime = new TextureObject("resources/textures/slime2-mod.png");
    pTexBackground = new TextureObject("resources/textures/dark_wood_background2.png");

    pText = new TextObject(ft, "resources/fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf");

    // caricamento modelli blender
    pCubeModel = new Model("resources/models/cubetto.obj");
    pCauldronModel = new Model("resources/models/cauldron.obj");
    pSlimeModel = new Model("resources/models/slime2.obj");
    pBackgroundModel = new Model("resources/models/background.obj");

    //caricamento modelli 2d
    pKeyTex = new TextureObject("resources/textures/test.png");
    pCandiesMesh.emplace_back(new TextureObject("resources/textures/candy_pink.png"));
    pCandiesMesh.emplace_back(new TextureObject("resources/textures/candy_green.png"));
    pCandiesMesh.emplace_back(new TextureObject("resources/textures/candy_blue.png"));
    pCandiesMesh.emplace_back(new TextureObject("resources/textures/candy_orange.png"));
    pCandiesMesh.emplace_back(new TextureObject("resources/textures/candy_violet.png"));

    pCandyTypes.emplace_back(new CandyType(EffectType::NoJump, 10.0f));
    pCandyTypes.emplace_back(new CandyType(EffectType::Speed, 1.5f, 10.0f));
    pCandyTypes.emplace_back(new CandyType(EffectType::SpeedEnemy, 1.5f, 10.0f));
    pCandyTypes.emplace_back(new CandyType(EffectType::Invincibility, 5.0f));
    pCandyTypes.emplace_back(new CandyType(EffectType::Teleport));

    pProbabilities.emplace_back(0);
    pProbabilities.emplace_back(0);
    pProbabilities.emplace_back(0);
    pProbabilities.emplace_back(0);
    pProbabilities.emplace_back(50);

    //Associazione run-time tra texture ed effetto della caramella
    std::shuffle(pCandiesMesh.begin(), pCandiesMesh.end(), rd);

    printf("TAB CARAMELLE\n");
    for (int i = 0; i < std::min(pCandiesMesh.size(), pCandyTypes.size()); i++)
        printf("%d\t%s\n", pCandyTypes[i]->effect, pCandiesMesh[i]->Path);

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

    pBackground = new GameObject(glm::vec2(0.0f, 0.0f), glm::vec3(1.5f, 1.5f, 1.5f), pTexBackground, 0);

    glm::mat4 projection2 = glm::ortho(0.0f, static_cast<float>(fbWidth), 0.0f, static_cast<float>(fbHeight));//left, right, bottom, top

    float left = -1.0f;   // Puoi modificare questi valori per adattarli alla tua scena
    float right = 1.0f;
    float bottom = -1.0f;
    float top = 1.0f;

    glm::mat4 projectionNDC = glm::ortho(left, right, bottom, top);
    glm::mat4 projectionPixels = glm::ortho(0.0f, static_cast<float>(fbWidth), 0.0f, static_cast<float>(fbHeight));//left, right, bottom, top

    glm::mat4 view = pCamera->GetViewMatrix();

    // setup delle uniform delle shader che non cambieranno nel ciclo di rendering
    // Shader base
    pShader->use();
    pShader->setMat4("projection", projectionNDC);
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

    // valori per materiale dorato
    pEnlightenedShader->setVec3("material.ambient", glm::vec3(0.24725f, 0.1995f, 0.0745f));
    pEnlightenedShader->setVec3("material.diffuse", glm::vec3(0.75164f, 0.60648f, 0.22648f));
    pEnlightenedShader->setVec3("material.specular", glm::vec3(0.628281f, 0.555802f, 0.366065f));
    pEnlightenedShader->setFloat("material.shininess", 128.0f * 0.4f);

    // posizione fonte di luce
    pEnlightenedShader->setVec3("light.position", glm::vec3(0.0f, -0.75f, 1.2f));

    // parametri luce
    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
    pEnlightenedShader->setVec3("light.ambient", lightColor * glm::vec3(0.3f));
    pEnlightenedShader->setVec3("light.diffuse", lightColor * glm::vec3(0.6f));
    pEnlightenedShader->setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

    // musica di sottofondo
    engine->play2D("resources/sounds/ost.wav", true);

    // cattura il tempo iniziale di gioco
    startTime = glfwGetTime();
}
PlayState::~PlayState() {

    delete pTexPlatforms;
    delete pTexPlayer;
    delete pTexEnemy;
    delete pCamera;
    delete pShader;
    delete pTextShader;
    delete pEnlightenedShader;
    delete pText;
    delete pCubeModel;
    delete pCauldronModel;
    delete pGretel;
    //delete pEnemy;
    //delete pCauldron_right;
    //delete pCauldron_left;

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
    if (CurrentLevel == 3)
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
    glm::vec2 posKey = {0.0f, 0.64f};
    glm::vec3 sizeKey = { 0.1f, 0.1f, 0.1f };

    CurrentScore = 0;

    if (pGretel != nullptr)
        delete pGretel;
    if (pHansel != nullptr)
        delete pHansel;
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
    nEnemies = 0;
    if (!pCandies.empty()) {
        for (Candy* pc : pCandies)
            delete pc;
        pCandies.clear();
    }

    /*
    pGretel = new Player(glm::vec2(0.0f, -0.75f), glm::vec3(0.1f, 0.1f, 0.1f), pCubeModel, pTexPlayer, 0, PlayerName::Gretel);
    if(Multiplayer)
        pHansel = new Player(glm::vec2(0.0f, -0.75f), glm::vec3(0.1f, 0.1f, 0.1f), pCubeModel, pTexPlayer, 0, PlayerName::Hansel);
    */

    pGretel = new Player(glm::vec2(0.0f, -0.85f), glm::vec3(0.12f, 0.12f * getAspect(Window) * pTexHansel->getAspect(), 0.1f), pTexGretel, 0, PlayerName::Gretel);
    if (Multiplayer)
        pHansel = new Player(glm::vec2(0.0f, -0.85f), glm::vec3(0.12f, 0.12f * getAspect(Window) * pTexHansel->getAspect(), 0.1f), pTexHansel, 0, PlayerName::Hansel);

    if (CurrentLevel == 1) {
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
    else if (CurrentLevel == 2) {
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
        if (pKey == nullptr)
            pKey = new GameObject(posKey, sizeKey, pKeyTex, 0);
    }

    // musica di sottofondo
    Engine->play2D("resources/sounds/ost.wav", true);

    lastSpawnTime = 0.0f;
    spawnTime = 0;
    spawnPlace = RandomInt(0, pCauldrons.size() - 1);
    printf("Prossimo spawn tra %d s nel calderone %d\n", spawnTime, spawnPlace);

    if (Status != GameStatus::Paused) {
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

    // debug
    if (glfwGetKey(Window, GLFW_KEY_C) == GLFW_PRESS)
    {
        std::cout << ((pGretel->isOnGround == true) ? "A terra" : "In aria") << std::endl;
        std::cout << "Velocity: " << pGretel->velocity.x << ", " << pGretel->velocity.y << std::endl;
        std::cout << "Bottom-Left: " << pGretel->Position.x - (pGretel->Size.x / 2) << ", " << pGretel->Position.y - (pGretel->Size.y / 2) << std::endl;
        std::cout << "HB-Bottom-Left: " << pGretel->GetHitbox().Min.x << ", " << pGretel->GetHitbox().Min.y << std::endl;
        std::cout << "HB-Top-Right: " << pGretel->GetHitbox().Max.x << ", " << pGretel->GetHitbox().Max.y << std::endl;
        std::cout << "Position-X: " << pGretel->Position.x << std::endl;
    }

    if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        Status = GameStatus::Paused;
        ChangeState(MenuState::GetInstance(Manager, Window, Engine));
    }

    ProcessInputPlayer(pGretel, GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_A, GLFW_KEY_D);
    if(Multiplayer)
        ProcessInputPlayer(pHansel, GLFW_KEY_UP, GLFW_KEY_DOWN, GLFW_KEY_LEFT, GLFW_KEY_RIGHT);

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
        Candy* pCloser = nullptr;
        float d_min = FLT_MAX;
        //Cerca se esiste la caramella da mangiare a distanza minima
        for (Candy* pc : pCandies) {
            if (!pc->IsEaten() && pPlayer->CheckCandyCollision(pc, Engine)) {
                float d = pPlayer->DistanceTo(pc);
                if (d < d_min) {
                    d_min = d;
                    pCloser = pc;
                }
            }
        }

        if (pCloser != nullptr) {
            pCloser->Eat();
            pPlayer->EatCandy(pCloser->GetType());
            printf("Player ha mangiato una caramella misteriosa\n");
        }
        canPressDOWN = false;
    }
    else if (glfwGetKey(Window, DOWN) == GLFW_RELEASE)
        canPressDOWN = true;

    pPlayer->Move(deltaTime);
    pPlayer->CheckCollisionWithSolids(platforms);

    if (CurrentLevel == 2 && pPlayer->CheckCollision(pKey) != MovingObject::Collision::None)
        Status = GameStatus::Victory;

    pPlayer->Update(deltaTime); // Aggiorna lo stato del giocatore
}
void PlayState::ProcessEvents() {

    if (pEnemies.size() < TOTENEM) {
        lastSpawnTime += deltaTime;

        if (lastSpawnTime >= spawnTime) {
#if FLAT
            if (CurrentLevel == 1)
                pEnemies.emplace_back(new Enemy(posSpawn[spawnPlace], glm::vec3(0.13f * pTexSlime->getAspect(), 0.13f , 0.1f), pTexSlime, 0, velocities[spawnPlace], true));
            else if (CurrentLevel == 2)
                pEnemies.emplace_back(new Enemy(posSpawn2[spawnPlace], glm::vec3(0.13f * pTexSlime->getAspect(), 0.13f, 0.1f), pTexSlime, 0, velocities2[spawnPlace], true));
#else
            if (CurrentLevel == 1)
                pEnemies.emplace_back(new Enemy(posSpawn[spawnPlace], glm::vec3(0.1f, 0.1f, 0.1f), pSlimeModel, pTexSlime, 0, velocities[spawnPlace], true));
            else if (CurrentLevel == 2)
                pEnemies.emplace_back(new Enemy(posSpawn2[spawnPlace], glm::vec3(0.1f, 0.1f, 0.1f), pSlimeModel, pTexSlime, 0, velocities2[spawnPlace], true));
#endif
            nEnemies++;
            lastSpawnTime = 0.0f;
            spawnTime = RandomInt(SPAWN_MIN_E, SPAWN_MAX_E);
            spawnPlace = RandomInt(0, pCauldrons.size() - 1);
            printf("Prossimo spawn tra %d s nel calderone %d\n", spawnTime, spawnPlace);
        }
    }

    for (Enemy* pe : pEnemies) {
        if (pe->IsDead() == false) {

            if (pGretel->CheckEnemyCollision(pe, Engine) || (Multiplayer && pHansel->CheckEnemyCollision(pe, Engine))) {//Se un player muore
                Status = GameStatus::GameOver;
            }
            else if (pe->IsDead()) {//Se enemy muore
                nEnemies--;
                printf("Nemico morto a %f, %f\n", pe->Position.x, pe->Position.y);
                if (CurrentLevel==1 && nEnemies <= 0 && pEnemies.size() == TOTENEM) {
                    Status = GameStatus::Victory;
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
                    pCandies.emplace_back(new Candy(glm::vec2(pixelX, pixelY), glm::vec3(0.07f, 0.07f * getAspect(Window) * texture->getAspect(), 0.1f), texture, 0, *type));
                    printf("Spawnata caramella in posizione %f %f con texture %s di tipo %d\n", pixelX, pixelY, texture->Path, type->effect);
                }

            }
            else {
                pe->Move(deltaTime);  // Aggiorna la posizione del nemico con controllo delle collisioni
                pe->CheckCollisionWithSolids(platforms);
            }
        }
    }

    if (pCandies.size() != 0)
        for (Candy* pc : pCandies) {
            if (!pc->IsEaten()) {
                pc->CheckCollisionWithSolids(platforms);
                pc->Move(deltaTime);
            }
        }

    if (Status == GameStatus::GameOver) {
        //reset !
        ChangeState(EndState::GetInstance(Manager, Window, Engine));
    }
    else if (Status == GameStatus::Victory) {
        CurrentLevel++;
        ChangeState(EndState::GetInstance(Manager, Window, Engine));
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
    glDepthMask(GL_FALSE);

    if (CurrentLevel == 2)
        pKey->Render(*pSpriteShader);

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

    pGretel->Render(*pSpriteShader);
    if(Multiplayer)
        pHansel->Render(*pSpriteShader);

    glDepthMask(GL_TRUE); // riattiva depth buffer
    glEnable(GL_DEPTH_TEST);

    for (GameObject* pc : pCauldrons)
        pc->Render(*pEnlightenedShader);

    // render testo per ultimo per essere davanti a tutto
    RenderStats();
}
void PlayState::MouseMoving(double xpos, double ypos)
{

}
void PlayState::MouseClick(int button, int action, int mods)
{
    if (pGretel->teleport && button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        //Ottiene coordinate pixel
        double xpos, ypos;
        int fbWidth, fbHeight;
        glfwGetCursorPos(Window, &xpos, &ypos);
        glfwGetFramebufferSize(Window, &fbWidth, &fbHeight);
        ypos = fbHeight - ypos;
        //Controlla se stia teleportando fuori schermo
        if (xpos <= 0 || xpos > fbWidth || ypos <= 0 || ypos > fbHeight) {
            printf("Coordinate fuori schermo -> RIPROVARE\n");
            return;
        }
        //Trasforma in coordinate NDC
        //xpos = (xpos + 1.0f) * 0.5f * fbWidth;
        //ypos = (ypos + 1.0f) * 0.5f * fbHeight;
        xpos = 2*xpos / fbWidth - 1.0f;
        ypos = 2*ypos / fbHeight - 1.0f;
        //Controlla se stia teleportando in area vietata
        for (GameObject* pp : platforms) {
            Hitbox bounds = pp->GetHitbox();
            bool isColliding = (xpos <= bounds.Max.x && xpos >= bounds.Min.x && ypos <= bounds.Max.y && ypos >= bounds.Min.y);
            if (isColliding) {
                printf("Coordinate collidono con ostacoli -> RIPROVARE\n");
                return;
            }
        }
        //Se le coordinate sono corrette teleporta
        pGretel->Teleport(glm::vec2(xpos,ypos));
        printf("Teleport effettuato\n");
    }
}
void PlayState::EnterState()
{
    //End of the game
    if (CurrentLevel == 3) {
        ChangeState(EndState::GetInstance(Manager, Window, Engine));
        return;
    }
        
    // musica di sottofondo
    Engine->play2D("resources/sounds/ost.wav", true);

    if (Status == GameStatus::Paused)//salta prima chiamata
        totalPauseTime += glfwGetTime() - startPauseTime;

    Status = GameStatus::Playing;
}

void PlayState::LeaveState() {
    if(Status == GameStatus::Paused)
        startPauseTime = glfwGetTime();
}

void PlayState::RenderStats() {
    int fbWidth, fbHeight;
    glfwGetFramebufferSize(Window, &fbWidth, &fbHeight);

    currentTime = static_cast<int>((glfwGetTime() - startTime) - totalPauseTime);

    int remainingTime = start - currentTime;

    // se scade il tempo perdo e chiudo il gioco
    if (remainingTime <= 0)
    {
        Status = GameStatus::GameOver;
    }

    /*
    std::string time = "TIME: " + std::to_string(remainingTime);
    pText->Render(*pTextShader, time, 200.0f, 1100.0f, 1.0f, glm::vec3(255.0, 255.0, 255.0));

    std::string lives = "LIVES: " + std::to_string(pPlayer->lives);
    pText->Render(*pTextShader, lives, 1200.0f, 1100.0f, 1.0f, glm::vec3(255.0, 255.0, 255.0));*/

    std::string time = "TIME: " + std::to_string(remainingTime);
    pText->Render(*pTextShader, time, 160.0f, 880.0f, 1.0f, glm::vec3(255.0, 255.0, 255.0));

    std::string lives = "LIVES: " + std::to_string(pGretel->lives);
    pText->Render(*pTextShader, lives, 960.0f, 880.0f, 1.0f, glm::vec3(255.0, 255.0, 255.0));

    std::string level = "LEVEL " + std::to_string(CurrentLevel);
    pText->Render(*pTextShader, level, fbWidth * 0.4f, 880.0f, 1.3f, glm::vec3(255.0, 255.0, 255.0));

}
