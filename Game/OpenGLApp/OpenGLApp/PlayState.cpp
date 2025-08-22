#include "PlayState.h"

#define NP 1
#define SPAWN_MIN_E 3
#define SPAWN_MAX_E 6

#define SPAWN_PROB_C 25 //percentuale di spawn delle caramelle (bonus e malus)

const glm::vec2 spawnPos[2] = { {-0.8f, 0.80f}, {0.8f, 0.80f} };
const glm::vec2 velocity = { 0.3f, 0.0f };
const glm::vec2 velocities[2] = { velocity, -velocity };

PlayState::PlayState(StateManager* manager, GLFWwindow* window, irrklang::ISoundEngine* engine)
    : GameState(manager, window, engine), lastFrame(0.0f), deltaTime(0.0f), nEnemies(0), CurrentLevel(1)
{
    int fbWidth, fbHeight;
    glfwGetFramebufferSize(Window, &fbWidth, &fbHeight);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    glm::vec2 positions[8] = {
       {0.0f, -0.95f},
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

    glm::vec3 sizesTest[8] = {
        {0.3f, 0.1f, 0.1f},
        {0.3f, 0.1f, 0.1f}, {0.3f, 0.1f, 0.1f},
        {0.3f, 0.1f, 0.1f}, {0.3f, 0.1f, 0.1f},
        {0.3f, 0.1f, 0.1f},
        {0.3f, 0.1f, 0.1f}, {0.3f, 0.1f, 0.1f}
    };

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
    pTexPlayer = new TextureObject("resources/textures/ice_cream_block.jpg");
    pTexEnemy = new TextureObject("resources/textures/awesomeface.png");
    pTexSlime = new TextureObject("resources/textures/slime2-mod.png");
    pTexBackground = new TextureObject("resources/textures/dark_wood_background2.png");

    pCamera = new Camera(glm::vec3(0.0f, 0.0f, 0.0f));

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

    pText = new TextObject(ft, "resources/fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf");

    // caricamento modelli blender
    pCubeModel = new Model("resources/models/cubetto.obj");
    pCauldronModel = new Model("resources/models/cauldron.obj");
    pSlimeModel = new Model("resources/models/slime2.obj");
    pBackgroundModel = new Model("resources/models/background.obj");

    //caricamento modelli 2d
    pCandiesMesh.emplace_back(new FlatMesh("resources/textures/candy1.png"));
    pCandiesMesh.emplace_back(new FlatMesh("resources/textures/candy2.png"));

    for (int i = 0; i < 8; ++i)
    {
        platforms.emplace_back(positions[i], sizes[i], pCubeModel, pTexPlatforms, 1);
    }
    /*

    pTest = new FlatMesh("resources/textures/test.png");

    float l = fbWidth / 20;

    tests.emplace_back(glm::vec2(fbWidth/ 2, l/2), glm::vec3(l*20.0f, l, 0.0f), pTest, 1);
    tests.emplace_back(glm::vec2(fbWidth/5, 3*l + l/2), glm::vec3(l * 8.0f, l, 0.0f), pTest, 1);
    tests.emplace_back(glm::vec2(fbWidth*4/5, 3*l +l/2), glm::vec3(l * 8.0f, l, 0.0f), pTest, 1);
    tests.emplace_back(glm::vec2(fbWidth / 5, 3 * l + l / 2), glm::vec3(l * 3.0f, l, 0.0f), pTest, 1);
    tests.emplace_back(glm::vec2(fbWidth * 4 / 5, 3 * l + l / 2), glm::vec3(l * 3.0f, l, 0.0f), pTest, 1);
    


    for (const GameObject& object : tests)
    {
        object.Print();
    }*/

    pBackground = new GameObject(glm::vec2(0.0f, 0.0f), glm::vec3(1.5f, 1.5f, 1.5f), pBackgroundModel, pTexBackground, 0);
    
    // passo nullptr come texture per ora
    pCauldrons.emplace_back(new GameObject(glm::vec2(0.89f, 0.64f), glm::vec3(0.1f, 0.1f, 0.1f), pCauldronModel, nullptr, 0));
    pCauldrons.emplace_back(new GameObject(glm::vec2(-0.89f, 0.64f), glm::vec3(0.1f, 0.1f, 0.1f), pCauldronModel, nullptr, 0));

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
    delete pPlayer;
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
    CurrentScore = 0;

    Status = GameStatus::Playing;

    if(pPlayer != nullptr)
        delete pPlayer;

    if (pEnemies.size() != 0)
        for (Enemy* pe : pEnemies)
            delete pe;

    if(pCandies.size() != 0)
        for (GameObject* pc : pCandies)
            delete pc;

    pPlayer = new Player(glm::vec2(0.0f, -0.75f), glm::vec3(0.1f, 0.1f, 0.1f), pCubeModel, pTexPlayer, 0);

    /*
    pEnemies.emplace_back(new Enemy(glm::vec2(-0.8f, 0.80f), glm::vec3(0.1f, 0.1f, 0.1f), pSlimeModel, pTexSlime, 0, glm::vec2(0.3f, 0.0f), true));
    pEnemies.emplace_back(new Enemy(glm::vec2(0.8f, 0.80f), glm::vec3(0.1f, 0.1f, 0.1f), pSlimeModel, pTexSlime, 0, glm::vec2(-0.3f, 0.0f), false));
    nEnemies++;
    nEnemies++;*/

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

void PlayState::ProcessInput()
{
  
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;

    if (deltaTime > 0.1f) { // Ignora delta troppo grandi
        deltaTime = 0.0f;
    }

    lastFrame = currentFrame;

    // debug
    if (glfwGetKey(Window, GLFW_KEY_C) == GLFW_PRESS)
    {
        std::cout << ((pPlayer->isOnGround == true) ? "A terra" : "In aria") << std::endl;
        std::cout << "Velocity: " << pPlayer->velocity.x << ", " << pPlayer->velocity.y << std::endl;
        std::cout << "Bottom-Left: " << pPlayer->Position.x - (pPlayer->Size.x / 2) << ", " << pPlayer->Position.y - (pPlayer->Size.y / 2) << std::endl;
        std::cout << "HB-Bottom-Left: " << pPlayer->GetHitbox().Min.x << ", " << pPlayer->GetHitbox().Min.y << std::endl;
        std::cout << "HB-Top-Right: " << pPlayer->GetHitbox().Max.x << ", " << pPlayer->GetHitbox().Max.y << std::endl;
        std::cout << "Position-X: " << pPlayer->Position.x << std::endl;
    }

    if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        Status = GameStatus::Paused;
        ChangeState(MenuState::GetInstance(Manager, Window, Engine));
    }
    // movimento orizzontale
    if (glfwGetKey(Window, GLFW_KEY_A) == GLFW_PRESS)
    {
        if (pPlayer->velocity.x > - pPlayer->maxVelocity.x)
        {
            pPlayer->velocity.x -= 0.01f;
        }
    }
    else if (glfwGetKey(Window, GLFW_KEY_D) == GLFW_PRESS)
    {
        if (pPlayer->velocity.x < pPlayer->maxVelocity.x)
        {
            pPlayer->velocity.x += 0.01f;
        }
    }
    else
    {
        pPlayer->velocity.x = 0.0f;
    }

    // salto
    if (glfwGetKey(Window, GLFW_KEY_W) == GLFW_PRESS)
    {
        pPlayer->HandleJump(deltaTime, Engine);
    }
    else
    {
        if (pPlayer->isOnGround)
        {
            pPlayer->isPastJumpPeak = true; // Se il tasto non è premuto, non si può più aumentare il salto
        }
    }

    pPlayer->Move(deltaTime);
    pPlayer->CheckCollisionWithSolids(platforms);

    pPlayer->Update(deltaTime); // Aggiorna lo stato del giocatore
}

void PlayState::ProcessEvents() {

    int fbWidth, fbHeight;
    glfwGetFramebufferSize(Window, &fbWidth, &fbHeight);
    float aspect = (float)fbWidth/(float)fbHeight;

    if (pEnemies.size() < TOTENEM) {
        lastSpawnTime += deltaTime;

        if (lastSpawnTime >= spawnTime) {
            pEnemies.emplace_back(new Enemy(spawnPos[spawnPlace], glm::vec3(0.1f, 0.1f, 0.1f), pSlimeModel, pTexSlime, 0, velocities[spawnPlace], true));
            nEnemies++;
            lastSpawnTime = 0.0f;
            spawnTime = RandomInt(SPAWN_MIN_E, SPAWN_MAX_E);
            spawnPlace = RandomInt(0, pCauldrons.size() - 1);
            printf("Prossimo spawn tra %d s nel calderone %d\n", spawnTime, spawnPlace);
        }
    }

    for (Enemy* pe : pEnemies) {
        if (pe->IsDead() == false) {

            if (pPlayer->CheckEnemyCollision(pe, Engine)) {//Se player muore
                Status = GameStatus::GameOver;
            }
            else if (pe->IsDead()) {//Se enemy muore
                nEnemies--;
                printf("Nemico morto a %f, %f\n", pe->Position.x, pe->Position.y);
                if (nEnemies <= 0 && pEnemies.size() == TOTENEM) {
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
                    pCandies.emplace_back(new Candy(glm::vec2(pixelX, pixelY), glm::vec3(0.07f, 0.07f * aspect, 0.1f), pCandiesMesh[0], 0));
                    printf("Spawnata caramella in posizione %f %f\n", pixelX, pixelY);
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
            pc->CheckCollisionWithSolids(platforms);
            pc->Move(deltaTime);
        }           

    if (Status == GameStatus::GameOver || Status == GameStatus::Victory) {
        //reset !
        ChangeState(EndState::GetInstance(Manager, Window, Engine));
    }
}

void PlayState::UpdateTime(long currentTime)
{
	
}

void PlayState::Render()
{
    //std::cout << "Rendering PlayState" << std::endl;

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

    for (const GameObject& object : platforms)
    {
        object.Render(*pShader);
    }

    // disattiva depth buffer quando si disegnano oggetti trasparenti (interferisce con blending)
    glDepthMask(GL_FALSE);

    if(pCandies.size() > 0){
        for (GameObject* pc : pCandies) {
            pc->Render(*pSpriteShader);
        }
    }

    if(pEnemies.size() > 0) {
        for (Enemy* pe : pEnemies) {
            if (!pe->IsDead()) {
                pe->Render(*pShader);
                //printf("Renderizzo nemico %d, morto? %d\n", i + 1, pEnemies[i]->IsDead());
            }
        }
    }

    pPlayer->Render(*pShader);

    glDepthMask(GL_TRUE); // riattiva depth buffer

    glEnable(GL_DEPTH_TEST);

    for (GameObject* pc : pCauldrons)
        pc->Render(*pEnlightenedShader);

    // render testo per ultimo per essere davanti a tutto
    // --------------------------------------------------

    RenderStats();
}

void PlayState::EnterState()
{
    // musica di sottofondo
    Engine->play2D("resources/sounds/ost.wav", true);

    if(Status == GameStatus::Paused)//salta prima chiamata
        totalPauseTime += glfwGetTime() - startPauseTime;

    Status = GameStatus::Playing;
}

void PlayState::LeaveState() {
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

    std::string lives = "LIVES: " + std::to_string(pPlayer->lives);
    pText->Render(*pTextShader, lives, 960.0f, 880.0f, 1.0f, glm::vec3(255.0, 255.0, 255.0));

    std::string level = "LEVEL " + std::to_string(CurrentLevel);
    pText->Render(*pTextShader, level, fbWidth*0.4f, 880.0f, 1.3f, glm::vec3(255.0, 255.0, 255.0));


}
