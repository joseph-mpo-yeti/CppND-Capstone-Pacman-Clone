#include "GameManager.h"

/* GameManager class implementation */

GameManager::GameManager()
{
    _graphics = std::make_unique<Graphics>();
}

GameManager::~GameManager()
{
    SDL_Quit();
}

void GameManager::run()
{
    _graphics->Init();
}