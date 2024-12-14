#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include "shader_s.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <irrKlang.h>

#include "GameObject.h"
#include "Player.h"
#include "Enemy.h"
#include "TextureObject.h"
#include "constants.h"

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

    // build and compile our shader zprogram
    // -------------------------------------
    Shader ourShader("shader.vs", "shader.fs");

    // inizializzo l'engine per i suoni
    // --------------------------------
    irrklang::ISoundEngine* engine = irrklang::createIrrKlangDevice();
    if (!engine)
        return 0; // error starting up the engine

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    std::vector<GameObject> platforms;

    glm::vec2 positions[] = {
        {0.0f, -0.9f}, 
        {-0.6f, -0.5f}, {0.6f, -0.5f},
        {-0.85f, -0.1f}, {0.85f, -0.1f}, 
        {0.0f, 0.0f},
        {-0.6f, 0.5f}, {0.6f, 0.5f}
    };

    glm::vec2 sizes[] = {
        {2.0f, 0.2f}, 
        {0.8f, 0.2f}, {0.8f, 0.2f},
        {0.3f, 0.2f}, {0.3f, 0.2f}, 
        {1.0f, 0.2f}, 
        {0.8f, 0.2f}, {0.8f, 0.2f}
    };

    TextureObject texPlatforms("textures/donut_block.jpg");
    TextureObject texPlayer("textures/ice_cream_block.jpg");
    TextureObject texEnemy("textures/awesomeface.png");

    for (int i = 0; i < 8; ++i) 
    {
        platforms.emplace_back(positions[i], sizes[i], texPlatforms, 1);
    }

    Player myPlayer(glm::vec2(-0.5f, -0.75f), glm::vec2(0.1f, 0.1f), texPlayer, 0);
    Enemy myEnemy(glm::vec2(0.0f, 0.15f), glm::vec2(0.1f, 0.1f), texEnemy, 0);

    float lastFrame = 0.0f;
    float deltatime = 0.0f;

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltatime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window, myPlayer, deltatime, engine);

        myPlayer.Move(deltatime);
        myPlayer.CheckCollision(platforms);

        if (myPlayer.CheckCollision(myEnemy)) {
            // se il giocatore muore chiude il gioco
            glfwSetWindowShouldClose(window, true);
        }

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
   
        for(const GameObject& object : platforms)
        {
            object.Render(ourShader);
        }

        myPlayer.Render(ourShader);
        myEnemy.Render(ourShader);
       
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

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window, Player& player, float deltatime, irrklang::ISoundEngine* engine)
{
    // debug
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) 
    {
        std::cout << ((player.isMidAir == true) ? "In aria" : "A terra") << std::endl;
        std::cout << "Velocity: " << player.Velocity.x << ", " << player.Velocity.y << std::endl;
        std::cout << "Bottom-Left: " << player.Position.x - (player.Size.x/2) << ", " << player.Position.y - (player.Size.y / 2) << std::endl;
    }
 
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // movimento orizzontale
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) 
    {
        if (player.Velocity.x > -player.MaxVelocity.x)
        {
            player.Velocity.x -= 0.01f;
        }
    }   
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        if (player.Velocity.x < player.MaxVelocity.x)
        {
            player.Velocity.x += 0.01f;
        }
    }
    else
    {
        player.Velocity.x = 0.0f;
    }

    // salto
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        player.HandleJump(deltatime, engine);
    }
    else
    {
        if (player.isMidAir) 
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