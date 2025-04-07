#include "PlayState.h"

PlayState::PlayState(StateManager* manager, Player* player)
    : GameState(manager), pPlayer(player)
{
}

void PlayState::Reset()
{
	CurrentLevel = 0;
    CurrentScore = 0;
	GameOver = false;
}

void PlayState::ProcessInput(GLFWwindow* window, float deltatime, irrklang::ISoundEngine* engine)
{
    // debug
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
    {
        std::cout << ((this->pPlayer->isOnGround == true) ? "A terra" : "In aria") << std::endl;
        std::cout << "Velocity: " << this->pPlayer->velocity.x << ", " << this->pPlayer->velocity.y << std::endl;
        std::cout << "Bottom-Left: " << this->pPlayer->Position.x - (this->pPlayer->Size.x / 2) << ", " << this->pPlayer->Position.y - (this->pPlayer->Size.y / 2) << std::endl;
        std::cout << "HB-Bottom-Left: " << this->pPlayer->GetHitbox().Min.x << ", " << this->pPlayer->GetHitbox().Min.y << std::endl;
        std::cout << "HB-Top-Right: " << this->pPlayer->GetHitbox().Max.x << ", " << this->pPlayer->GetHitbox().Max.y << std::endl;
        std::cout << "Position-X: " << this->pPlayer->Position.x << std::endl;
    }

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // movimento orizzontale
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        if (this->pPlayer->velocity.x > -this->pPlayer->maxVelocity.x)
        {
            this->pPlayer->velocity.x -= 0.01f;
        }
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        if (this->pPlayer->velocity.x < this->pPlayer->maxVelocity.x)
        {
            this->pPlayer->velocity.x += 0.01f;
        }
    }
    else
    {
        this->pPlayer->velocity.x = 0.0f;
    }

    // salto
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        this->pPlayer->HandleJump(deltatime, engine);
    }
    else
    {
        if (!this->pPlayer->isOnGround)
        {
            this->pPlayer->isPastJumpPeak = true; // Se il tasto non è premuto, non si può più aumentare il salto
        }
    }
}

void PlayState::UpdateTime(long currentTime)
{
	if (!GameOver)
	{

	}
}

void PlayState::Draw()
{
}