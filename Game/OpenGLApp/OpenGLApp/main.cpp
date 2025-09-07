#include <iostream>
#include <vector>
#include <string>
#include <chrono>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <irrKlang.h>

#include "stb_image.h"
#include "shader_s.h"
#include "camera.h"
#include "model.h"
#include "utils.h"

#include "StateManager.h"
#include "MenuState.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void cursor_moving_callback(GLFWwindow* window, double xpos, double ypos);
void cursor_button_callback(GLFWwindow* window, int button, int action, int mods);

// Definisco lo StateManager che si occupa di gestire gli stati di gioco
StateManager mySManager;

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

    //inizializza generatore numeri casuali
    RandomSetup();

    // Spawna la finestra al centro dello schermo
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    int screenWidth = mode->width;
    int screenHeight = mode->height;

    int xpos = (screenWidth - SCR_WIDTH) / 2;
    int ypos = 50;

    glfwSetWindowPos(window, xpos, ypos);

    int fbWidth, fbHeight;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
    // -----------------------------------------------------------------------------------------

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetCursorPosCallback(window, cursor_moving_callback);
    glfwSetMouseButtonCallback(window, cursor_button_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    // inizializzo l'engine per i suoni
    // --------------------------------
    irrklang::ISoundEngine* engine = irrklang::createIrrKlangDevice();
    if (!engine) {
        std::cout << "ERROR::IRRKLANG: Could not init irrKlang Engine" << std::endl; // error starting up the engine
        return 0;
    }

    //MenuState myPState(&mySManager, window, engine);
    //mySManager.ChangeState(&myPState);
    mySManager.ChangeState(MenuState::GetInstance(&mySManager, window, engine));

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        //il processamento e il rendering avvengono sempre tramite il manager che conosce il "contesto" di gioco
        mySManager.ProcessInput();
        mySManager.ProcessEvents();
        mySManager.Render();

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

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void cursor_moving_callback(GLFWwindow* window, double xpos, double ypos) {
    mySManager.MouseMoving(xpos, ypos);
}
void cursor_button_callback(GLFWwindow* window, int button, int action, int mods) {
    mySManager.MouseClick(button, action, mods);
}