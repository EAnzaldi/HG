#pragma once

#include<cstdlib>
#include<ctime>
#include <random>
#include <GLFW/glfw3.h>

//0 = no print, 1 = debug print
#define DEBUG 0
 
// screen's settings
//static const unsigned int SCR_WIDTH = 1600;
//static const unsigned int SCR_HEIGHT = 1200;
static const unsigned int SCR_WIDTH = 1280;
static const unsigned int SCR_HEIGHT = 960;
static const float SCR_WIDTH_F = static_cast<float>(SCR_WIDTH);
static const float SCR_HEIGHT_F = static_cast<float>(SCR_HEIGHT);

static float getAspect(GLFWwindow* window) {
    //int fbWidth, fbHeight;
    //glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
    return SCR_WIDTH_F / SCR_HEIGHT_F;
}

// random generator
const unsigned int seed = time(nullptr);

static void RandomSetup() {
    srand(seed);
}

static int RandomInt(int min, int max) {
    int n = std::rand();
    n = n % (max - min + 1);
    n += min;
    return n;
}

static bool DoAction(int probability) {
    int n = rand() % 100;
    //printf("Percentuale estratta %d\n", n);
    return n < probability;
}

static std::random_device rd;
static std::mt19937 gen(rd());

/*
static void RandomSetupAdvanced() {
    std::mt19937 gen(rd());
}*/