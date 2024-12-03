#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include "shader_s.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

#include "GameObject.h"
#include "Player.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, Player& player);
unsigned int LoadTexture(const char* path);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Demo - H&G", NULL, NULL);
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
    // ------------------------------------
    Shader ourShader("shader.vs", "shader.fs");

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

    unsigned int textureID = LoadTexture("textures/container.jpg");

    for (int i = 0; i < 8; ++i) 
    {
        platforms.emplace_back(positions[i], sizes[i], textureID);
    }

    Player myPlayer(glm::vec2(-0.5f, -0.75f), glm::vec2(0.1f, 0.1f), textureID);

    float lastFrame = 0.0f;
    float deltatime = 0.0f;


    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----

        float currentFrame = static_cast<float>(glfwGetTime());

        deltatime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window, myPlayer);
        myPlayer.Move(deltatime);
        myPlayer.CheckCollisions(platforms);


        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
   
        for(const GameObject& object : platforms)
        {
            object.Render(ourShader);
        }

        myPlayer.Render(ourShader);
       
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
void processInput(GLFWwindow* window, Player& player)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    //movimento orizzontale
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) 
    {
        player.Velocity.x -= 0.01f;
    }   
    else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        player.Velocity.x += 0.01f;
    }
    else
    {
        player.Velocity.x = 0.0f;
    }

    //movimento verticale
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        player.Velocity.y += 0.01f;
    }
    else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        player.Velocity.y -= 0.01f;
    }
    else
    {
        player.Velocity.y = 0.0f;
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

//da spostare in una classe utility
unsigned int LoadTexture(const char* path)
{
    unsigned int textureID;
    
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    return textureID;
}