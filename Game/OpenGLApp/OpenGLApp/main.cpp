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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "H+G", NULL, NULL);
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

    Camera camera(glm::vec3(0.0f, 0.0f, 1.0f));

    // build and compile our shader zprogram
    // -------------------------------------
    Shader ourShader("shader.vs", "shader.fs");
    Shader textShader("shader_text.vs", "shader_text.fs");
    Shader enlightenedShader("enlighted_object_shader.vs", "enlightened_object_shader.fs");

    // inizializzo l'engine per i suoni
    // --------------------------------
    irrklang::ISoundEngine* engine = irrklang::createIrrKlangDevice();
    if (!engine)
        return 0; // error starting up the engine

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
        {1.0f, 0.1f, 0.1f},
        {0.4f, 0.1f, 0.1f}, {0.4f, 0.1f, 0.1f},
        {0.15f, 0.1f, 0.1f}, {0.15f, 0.1f, 0.1f},
        {0.5f, 0.1f, 0.1f},
        {0.4f, 0.1f, 0.1f}, {0.4f, 0.1f, 0.1f}
    };

    TextureObject texPlatforms("resources/textures/donut_block.jpg");
    TextureObject texPlayer("resources/textures/ice_cream_block.jpg");
    TextureObject texEnemy("resources/textures/awesomeface.png");

    TextObject text(ft, "resources/fonts/8-bit-operator/8bitOperatorPlus8-Regular.ttf");

    Model cubeModel("resources/models/cubetto.obj");

    for (int i = 0; i < 8; ++i) 
    {
        platforms.emplace_back(positions[i], sizes[i], cubeModel, texPlatforms, 1);
    }

    Player myPlayer(glm::vec2(-0.5f, -0.75f), glm::vec3(0.05f, 0.05f, 0.05f), cubeModel, texPlayer, 0);
    
    Enemy myEnemy(glm::vec2(0.8f, 0.85f), glm::vec3(0.05f, 0.05f, 0.05f), cubeModel, texEnemy, 0, glm::vec2(-0.8f, 0.0f));

    float lastFrame = 0.0f;
    float deltatime = 0.0f;
    float rotationAngle = 0.0f; // Angolo di rotazione del modello

    double start = glfwGetTime();

    float left = -1.0f;   // Puoi modificare questi valori per adattarli alla tua scena
    float right = 1.0f;
    float bottom = -1.0f;
    float top = 1.0f;
    float nearPlane = 0.1f;
    float farPlane = 100.0f;

    glm::mat4 projection = glm::ortho(left, right, bottom, top);
    glm::mat4 view = camera.GetViewMatrix();

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltatime = currentFrame - lastFrame;

        if (deltatime > 0.1f) { // Ignora delta troppo grandi
            deltatime = 0.0f;
        }

        lastFrame = currentFrame;

        rotationAngle += deltatime * 50.0f; // Incrementa l'angolo di rotazione
        if (rotationAngle > 360.0f) {
            rotationAngle -= 360.0f;
        }

        processInput(window, myPlayer, deltatime, engine);

        myPlayer.Move(deltatime);
        myPlayer.CheckCollisionWithSolids(platforms);

        myPlayer.Update(deltatime); // Aggiorna lo stato del giocatore

        if (myPlayer.CheckEnemyCollision(myEnemy)) {
            // se il giocatore muore chiude il gioco
            glfwSetWindowShouldClose(window, true);
        }

        myEnemy.Move(deltatime);  // Aggiorna la posizione del nemico con controllo delle collisioni
        myEnemy.CheckCollisionWithSolids(platforms);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // render testo per ultimo per essere davanti a tutto
        // --------------------------------------------------

        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        for(const GameObject& object : platforms)
        {
            object.Render(ourShader);
        }
        
        //// render the loaded model (se lo si mette dietro a platforms perde la texture)
        //glm::mat4 model = glm::mat4(1.0f);
        //model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        //model = glm::rotate(model, glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        //model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
        //ourShader.setMat4("model", model);
        //cubeModel.Draw(ourShader);
        
        myPlayer.Render(ourShader);
        myEnemy.Render(ourShader);

        int t = static_cast<int>(glfwGetTime()) - static_cast<int>(start);
        std::string time = "TIME: " + std::to_string(t);
        text.Render(textShader, time, 200.0f, 1100.0f, 1.0f, glm::vec3(255.0, 255.0, 255.0));

        std::string lives = "LIVES: " + std::to_string(myPlayer.lives);
        text.Render(textShader, lives, 1200.0f, 1100.0f, 1.0f, glm::vec3(255.0, 255.0, 255.0));

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    //Distrugge FreeType
    FT_Done_FreeType(ft);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

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