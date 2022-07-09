#include <iostream>
#include <future>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <algorithm>
#include <chrono>
#include <SDL2/SDL.h>

#include "GameManager.h"

/* GameManager class implementation */

GameManager::GameManager()
{
    _player = std::make_unique<Entity>(Tag::PLAYER, Assets::image::PLAYER);
    _graphics = std::make_unique<Graphics>();
}

// bahavior
bool GameManager::Init()
{
    if(!_graphics->Init())
        return false;

    ShowLoading();
               
    // loading assets in separate threads
    auto mazeInitFuture = std::async(std::launch::async, &GameManager::InitMaze, this);
    auto enemiesInitFuture = std::async(std::launch::async, &GameManager::InitEnemies, this);
    auto playerInitFuture = std::async(std::launch::async, &GameManager::InitPlayer, this);

    if(!playerInitFuture.get() || !enemiesInitFuture.get() || !mazeInitFuture.get()){
        std::cout << "Failed to initialize game objects. " << std::endl;
        return false;
    }

    return true;
}

void GameManager::Run()
{
    HideLoading();
    
    ShowGame();

    SDL_Event event;

    auto lastUpdate = std::chrono::system_clock::now();
    int timeSinceLastUpdate = 0;

    _isRunning = true;

    while(_isRunning){

        
        timeSinceLastUpdate = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - lastUpdate).count();
        if(timeSinceLastUpdate < _graphics->GetFrameDuration()){
            std::this_thread::sleep_for(std::chrono::milliseconds(_graphics->GetFrameDuration() - timeSinceLastUpdate));
        }
        
        // skip updates if paused
        if(_isPaused)
            continue;

        ProcessInput(event);
        Update();
        Render();

        lastUpdate = std::chrono::system_clock::now();

    }
}

void GameManager::ProcessInput(SDL_Event& event)
{
    std::cout << "Polling events..." << std::endl;
    // polling events
    SDL_PollEvent(&event);

    // handling input
    switch (event.type) {
        case SDL_QUIT:
            EndGame();
            return;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym){
                case SDLK_SPACE:
                    if(_isPaused) 
                        Resume();
                    else 
                        Pause();
                    break;
                case SDLK_UP:
                    if(!_isPaused) _player->GetTransform().position.Update(0, -1, 0 );
                    break;
                case SDLK_DOWN:
                    if(!_isPaused) _player->GetTransform().position.Update(0, 1, 0);
                    break;
                case SDLK_LEFT:
                    if(!_isPaused) _player->GetTransform().position.Update(-1, 0, 0);
                    break;
                case SDLK_RIGHT:
                    if(!_isPaused) _player->GetTransform().position.Update(1, 0, 0);
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

void GameManager::Update()
{
    // Implement Collision detection
    // Update lives and score
    // Update transform
    // Update food and powerups
    // 
    std::cout << "Game updated" << std::endl;
}

void GameManager::Render()
{
    _graphics->Clear();

    _graphics->Render(*_player.get());

    std::for_each(_enemies.begin(), _enemies.end(), [this](std::unique_ptr<Entity>& enemy){
        this->_graphics->Render(*enemy.get());
    });

    _graphics->Present();
}

void GameManager::Pause()
{
    _isPaused = true;
}

void GameManager::ShowGame()
{

}

void GameManager::ShowLoading()
{

}

void GameManager::HideGame()
{

}

void GameManager::HideLoading()
{

}

void GameManager::Resume()
{
    _isPaused = false;
}

bool GameManager::InitPlayer()
{
    std::cout << "Player Initialized" << std::endl;
    auto texture = _graphics->LoadTexture(Assets::image::PLAYER);
    _player->LoadTexture(std::move(texture));
    return true;
}

bool GameManager::InitEnemies()
{   
    _enemies.emplace_back(std::make_unique<Entity>(Tag::ENEMY, Assets::image::ENEMY_YELLOW));
    _enemies.emplace_back(std::make_unique<Entity>(Tag::ENEMY, Assets::image::ENEMY_BLUE));
    _enemies.emplace_back(std::make_unique<Entity>(Tag::ENEMY, Assets::image::ENEMY_PINK));
    _enemies.emplace_back(std::make_unique<Entity>(Tag::ENEMY, Assets::image::ENEMY_RED));

    std::cout << "Enemies Initialized" << std::endl;

    return true;
}

bool GameManager::InitMaze()
{
    std::cout << "Maze Initialized" << std::endl;
    return true;
}

void GameManager::EndGame()
{
    _isRunning = false;
}