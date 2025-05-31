#include "PlayState.h"
#include "Player.h"
#include "MenuState.h"
#include "EndState.h"

PlayState::PlayState(StateManager* manager, GLFWwindow* window, irrklang::ISoundEngine* engine)
    : GameState(manager, window, engine), lastFrame(0.0f), deltaTime(0.0f), GameOver(false), Paused(false), nEnemies(TOTENEM)
{
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    glm::vec2 positions[8] = {
       {0.0f, -0.9f},
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

    pTexPlatforms = new TextureObject("resources/textures/donut_block.jpg");
    pTexPlayer = new TextureObject("resources/textures/ice_cream_block.jpg");
    pTexEnemy = new TextureObject("resources/textures/awesomeface.png");

    pCamera = new Camera(glm::vec3(0.0f, 0.0f, 0.0f));

    // build and compile our shader zprogram
    // -------------------------------------
    pShader = new Shader("shader.vs", "shader.fs");
    pTextShader = new Shader("shader_text.vs", "shader_text.fs");
    pEnlightenedShader = new Shader("enlightened_object_shader.vs", "enlightened_object_shader.fs");

    // Initializza FreeType
    if (FT_Init_FreeType(&ft)) {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }

    pText = new TextObject(ft, "resources/fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf");

    // caricamento modelli blender
    pCubeModel = new Model("resources/models/cubetto.obj");
    pCauldronModel = new Model("resources/models/cauldron.obj");

    for (int i = 0; i < 8; ++i)
    {
        platforms.emplace_back(positions[i], sizes[i], *pCubeModel, pTexPlatforms, 1);
    }
    
    pPlayer = new Player(glm::vec2(0.0f, -0.75f), glm::vec3(0.1f, 0.1f, 0.1f), *pCubeModel, pTexPlayer, 0);
    pEnemy = new Enemy(glm::vec2(-0.8f, 0.80f), glm::vec3(0.1f, 0.1f, 0.1f), *pCubeModel, pTexEnemy, 0, glm::vec2(0.8f, 0.0f));


    // passo nullptr come texture per ora
    pCauldron_right = new GameObject(glm::vec2(0.90f, -0.75f), glm::vec3(0.1f, 0.1f, 0.1f), *pCauldronModel, nullptr, 0);
    pCauldron_left = new GameObject(glm::vec2(-0.90f, -0.75f), glm::vec3(0.1f, 0.1f, 0.1f), *pCauldronModel, nullptr, 0);

    float left = -1.0f;   // Puoi modificare questi valori per adattarli alla tua scena
    float right = 1.0f;
    float bottom = -1.0f;
    float top = 1.0f;

    glm::mat4 projection = glm::ortho(left, right, bottom, top);
    glm::mat4 view = pCamera->GetViewMatrix();

    // setup delle uniform delle shader che non cambieranno nel ciclo di rendering
    pShader->use();
    pShader->setMat4("projection", projection);
    pShader->setMat4("view", view);

    pEnlightenedShader->use();
    pEnlightenedShader->setMat4("projection", projection);
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
    delete pEnemy;
    delete pCauldron_right;
    delete pCauldron_left;

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
    CurrentLevel = 0;
    CurrentScore = 0;
	GameOver = false;

    delete pPlayer;
    delete pEnemy;

    pPlayer = new Player(glm::vec2(0.0f, -0.75f), glm::vec3(0.1f, 0.1f, 0.1f), *pCubeModel, pTexPlayer, 0);
    pEnemy = new Enemy(glm::vec2(-0.8f, 0.80f), glm::vec3(0.1f, 0.1f, 0.1f), *pCubeModel, pTexEnemy, 0, glm::vec2(0.8f, 0.0f));

    // musica di sottofondo
    Engine->play2D("resources/sounds/ost.wav", true);

    if (!IsPaused()) {
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

    if (pEnemy->IsDead() == false) {

        if (pPlayer->CheckEnemyCollision(pEnemy, Engine)) {//Se player muore
            GameOver = true;
        }
        else if (pEnemy->IsDead()) {//Se enemy muore
            nEnemies--;
            if (nEnemies <= 0) {
                Victory = true;
            }
        }
        else {
            pEnemy->Move(deltaTime);  // Aggiorna la posizione del nemico con controllo delle collisioni
            pEnemy->CheckCollisionWithSolids(platforms);
        }     
    }

    if (GameOver) {
        //reset !
        GameState::Status = GameStatus::GameOver;
        ChangeState(EndState::GetInstance(Manager, Window, Engine));
    } else if (Victory) {
        //reset !
        GameState::Status = GameStatus::Victory;
        ChangeState(EndState::GetInstance(Manager, Window, Engine));
    }
}

void PlayState::UpdateTime(long currentTime)
{
	if (!GameOver)
	{

	}
}

void PlayState::Render()
{
    //std::cout << "Rendering PlayState" << std::endl;

    //Aggiunte le seguenti due righe per gestire correttamente la trasparenza
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (const GameObject& object : platforms)
    {
        object.Render(*pShader);
    }

    pPlayer->Render(*pShader);

    if(pEnemy->IsDead() == false)
        pEnemy->Render(*pShader);

    pCauldron_right->Render(*pEnlightenedShader);
    pCauldron_left->Render(*pEnlightenedShader);

    // render testo per ultimo per essere davanti a tutto
    // --------------------------------------------------

    currentTime = start - static_cast<int>((glfwGetTime() - startTime) - totalPauseTime);

    // se scade il tempo perdo e chiudo il gioco
    if (currentTime <= 0)
    {
        GameOver = true;
    }

    std::string time = "TIME: " + std::to_string(currentTime);
    pText->Render(*pTextShader, time, 200.0f, 1100.0f, 1.0f, glm::vec3(255.0, 255.0, 255.0));

    std::string lives = "LIVES: " + std::to_string(pPlayer->lives);
    pText->Render(*pTextShader, lives, 1200.0f, 1100.0f, 1.0f, glm::vec3(255.0, 255.0, 255.0));
}

void PlayState::EnterState()
{
    // musica di sottofondo
    Engine->play2D("resources/sounds/ost.wav", true);

    if(IsPaused())//salta prima chiamata
        totalPauseTime += glfwGetTime() - startPauseTime;

    Paused = false;
}

void PlayState::LeaveState() {

    startPauseTime = glfwGetTime();

    if(!GameOver)
        Paused = true;
}

void PlayState::RenderStats() {

}
