#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include "shader_s.h"
#include "camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>
#include <string>
#include <chrono>

#include <irrKlang.h>

#include "GameObject.h"
#include "Player.h"
#include "Enemy.h"
#include "TextureObject.h"
#include "TextObject.h"
#include "constants.h"
#include "model.h"
#include "StateManager.h"
#include "PlayState.h"

#define PREC 0
#define NEW !PREC

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, Player& player, float deltatime, irrklang::ISoundEngine* engine);

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "H&G", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    #if PREC

    Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));

    // build and compile our shader zprogram
    // -------------------------------------
    Shader ourShader("shader.vs", "shader.fs");
    Shader textShader("shader_text.vs", "shader_text.fs");
    Shader enlightenedShader("enlightened_object_shader.vs", "enlightened_object_shader.fs");

    #endif // PREC


    // inizializzo l'engine per i suoni
    // --------------------------------
    irrklang::ISoundEngine* engine = irrklang::createIrrKlangDevice();
    if (!engine) {
        std::cout << "ERROR::IRRKLANG: Could not init irrKlang Engine" << std::endl; // error starting up the engine
        return 0;
    }

    #if PREC

    // Initializza FreeType
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    std::vector<GameObject> platforms;

    std::vector<Enemy> enemies;

    glm::vec2 positions[] = {
        {0.0f, -0.9f}, 
        {-0.6f, -0.5f}, {0.6f, -0.5f},
        {-0.85f, -0.1f}, {0.85f, -0.1f}, 
        {0.0f, 0.0f},
        {-0.6f, 0.5f}, {0.6f, 0.5f}
    };

    glm::vec3 sizes[] = {
        {2.0f, 0.1f, 0.2f},
        {0.8f, 0.1f, 0.2f}, {0.8f, 0.1f, 0.2f},
        {0.3f, 0.1f, 0.2f}, {0.3f, 0.1f, 0.2f},
        {1.0f, 0.1f, 0.2f},
        {0.8f, 0.1f, 0.2f}, {0.8f, 0.1f, 0.2f}
    };


    TextureObject texPlatforms("resources/textures/donut_block.jpg");
    TextureObject texPlayer("resources/textures/ice_cream_block.jpg");
    TextureObject texEnemy("resources/textures/awesomeface.png");

    TextObject text(ft, "resources/fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf");

    // caricamento modelli blender
    Model cubeModel("resources/models/cubetto.obj");
    Model cauldronModel("resources/models/cauldron.obj");

    for (int i = 0; i < 8; ++i) 
    {
        platforms.emplace_back(positions[i], sizes[i], cubeModel, &texPlatforms, 1);
    }

    Player myPlayer(glm::vec2(0.0f, -0.75f), glm::vec3(0.1f, 0.1f, 0.1f), cubeModel, &texPlayer, 0);
    
    Enemy myEnemy(glm::vec2(-0.8f, 0.80f), glm::vec3(0.1f, 0.1f, 0.1f), cubeModel, &texEnemy, 0, glm::vec2(0.8f, 0.0f));

    // passo nullptr come texture per ora
    GameObject cauldron_right(glm::vec2(0.90f, -0.75f), glm::vec3(0.1f, 0.1f, 0.1f), cauldronModel, nullptr, 0);
    GameObject cauldron_left(glm::vec2(-0.90f, -0.75f), glm::vec3(0.1f, 0.1f, 0.1f), cauldronModel, nullptr, 0);

    float left = -1.0f;   // Puoi modificare questi valori per adattarli alla tua scena
    float right = 1.0f;
    float bottom = -1.0f;
    float top = 1.0f;

    glm::mat4 projection = glm::ortho(left, right, bottom, top);
    glm::mat4 view = camera.GetViewMatrix();

    // setup delle uniform delle shader che non cambieranno nel ciclo di rendering
    ourShader.use();
    ourShader.setMat4("projection", projection);
    ourShader.setMat4("view", view);

    enlightenedShader.use();
    enlightenedShader.setMat4("projection", projection);
    enlightenedShader.setMat4("view", view);

    enlightenedShader.setVec3("viewPos", camera.Position);

    // valori per materiale dorato
    enlightenedShader.setVec3("material.ambient", glm::vec3(0.24725f, 0.1995f, 0.0745f));
    enlightenedShader.setVec3("material.diffuse", glm::vec3(0.75164f, 0.60648f, 0.22648f));
    enlightenedShader.setVec3("material.specular", glm::vec3(0.628281f, 0.555802f, 0.366065f));
    enlightenedShader.setFloat("material.shininess", 128.0f * 0.4f);

    // posizione fonte di luce
    enlightenedShader.setVec3("light.position", glm::vec3(0.0f, -0.75f, 1.2f));

    // parametri luce
    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
    enlightenedShader.setVec3("light.ambient", lightColor * glm::vec3(0.3f));
    enlightenedShader.setVec3("light.diffuse", lightColor * glm::vec3(0.6f));
    enlightenedShader.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

    // musica di sottofondo
    engine->play2D("resources/sounds/ost.wav", true);
    

    // tempo massimo per livello
    double start = 99.0f;

    float lastFrame = 0.0f;
    float deltatime = 0.0f;

    #endif

    #if NEW
    // Inizializzo lo StateManager che si occupa di gestire gli stati di gioco
    StateManager mySManager;
    PlayState myPState(&mySManager, window, engine);
    mySManager.ChangeState(&myPState);
    #endif

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        #if PREC

        float currentFrame = static_cast<float>(glfwGetTime());
        deltatime = currentFrame - lastFrame;

        if (deltatime > 0.1f) { // Ignora delta troppo grandi
            deltatime = 0.0f;
        }

        lastFrame = currentFrame;

        #endif

        #if NEW
        //il processamento dell'input avviene sempre tramite il manager che conosce il "contesto" dei tasti
        mySManager.ProcessInput();
        mySManager.Render();
        #endif // PREC

        #if PREC

        processInput(window, myPlayer, deltatime, engine);

        myPlayer.Move(deltatime);
        myPlayer.CheckCollisionWithSolids(platforms);

        myPlayer.Update(deltatime); // Aggiorna lo stato del giocatore

        if (myPlayer.CheckEnemyCollision(myEnemy, engine)) {
            // se il giocatore muore chiude il gioco
            glfwSetWindowShouldClose(window, true);
        }

        myEnemy.Move(deltatime);  // Aggiorna la posizione del nemico con controllo delle collisioni
        myEnemy.CheckCollisionWithSolids(platforms);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for(const GameObject& object : platforms)
        {
            object.Render(ourShader);
        }
     
        myPlayer.Render(ourShader);
        myEnemy.Render(ourShader);

        cauldron_right.Render(enlightenedShader);
        cauldron_left.Render(enlightenedShader);

        // render testo per ultimo per essere davanti a tutto
        // --------------------------------------------------
        int t = static_cast<int>(start) - static_cast<int>(glfwGetTime());

        // se scade il tempo perdo e chiudo il gioco
        if (t <= 0)
        {
            glfwSetWindowShouldClose(window, true);
        }

        std::string time = "TIME: " + std::to_string(t);
        text.Render(textShader, time, 200.0f, 1100.0f, 1.0f, glm::vec3(255.0, 255.0, 255.0));

        std::string lives = "LIVES: " + std::to_string(myPlayer.lives);
        text.Render(textShader, lives, 1200.0f, 1100.0f, 1.0f, glm::vec3(255.0, 255.0, 255.0));

        #endif
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// Obsoleto, incorporato nella gestione degli stati:

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window, Player& player, float deltatime, irrklang::ISoundEngine* engine)
{
    // debug
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) 
    {
        std::cout << ((player.isOnGround == true) ? "A terra" : "In aria") << std::endl;
        std::cout << "Velocity: " << player.velocity.x << ", " << player.velocity.y << std::endl;
        std::cout << "Bottom-Left: " << player.Position.x - (player.Size.x/2) << ", " << player.Position.y - (player.Size.y / 2) << std::endl;
        std::cout << "HB-Bottom-Left: " << player.GetHitbox().Min.x << ", " << player.GetHitbox().Min.y << std::endl;
        std::cout << "HB-Top-Right: " << player.GetHitbox().Max.x << ", " << player.GetHitbox().Max.y << std::endl;
        std::cout << "Position-X: " << player.Position.x << std::endl;
    }
 
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // movimento orizzontale
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) 
    {
        if (player.velocity.x > -player.maxVelocity.x)
        {
            player.velocity.x -= 0.01f;
        }
    }   
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        if (player.velocity.x < player.maxVelocity.x)
        {
            player.velocity.x += 0.01f;
        }
    }
    else
    {
        player.velocity.x = 0.0f;
    }

    // salto
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        player.HandleJump(deltatime, engine);
    }
    else
    {
        if (!player.isOnGround) 
        {
            player.isPastJumpPeak = true; // Se il tasto non è premuto, non si può più aumentare il salto
        }
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}