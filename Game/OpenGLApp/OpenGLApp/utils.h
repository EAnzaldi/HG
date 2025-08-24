#pragma once

#include<cstdlib>
#include<ctime>
#include <random>

// screen's settings
// const unsigned int SCR_WIDTH = 1600;
//const unsigned int SCR_HEIGHT = 1200;
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 960;
const float SCR_WIDTH_F = static_cast<float>(SCR_WIDTH);
const float SCR_HEIGHT_F = static_cast<float>(SCR_HEIGHT);

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
    printf("Percentuale estratta %d\n", n);
    return n < probability;
}

static std::random_device rd;
static std::mt19937 gen(rd());

/*
static void RandomSetupAdvanced() {
    std::mt19937 gen(rd());
}*/