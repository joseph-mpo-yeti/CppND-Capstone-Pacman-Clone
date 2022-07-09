#include <iostream>
#include <future>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <algorithm>
#include <chrono>
#include <SFML/Graphics.hpp>

#include "GameManager.h"

/* GameManager class implementation */

GameManager::GameManager()
{
    _player = std::make_unique<Entity>(EntityType::PLAYER, EnemyTag::PLAYER);
    _graphics = std::make_unique<Graphics>();
}


GameManager::~GameManager()
{
    std::for_each(_threads.begin(), _threads.end(), [this](std::thread& tr){
        tr.join();
    });
}

GameManager::GameManager(GameManager&& other)
{
    if(this == &other)
        return;

    MoveHere(std::move(other));
}

GameManager& GameManager::operator=(GameManager&& other)
{
    if(this == &other)
        return *this;
        
    MoveHere(std::move(other));

    return *this;
}

void GameManager::MoveHere(GameManager&& other)
{
    _score = other._score;
    _isPaused = other._isPaused;
    _isRunning = other._isRunning;
    _graphics = std::move(other._graphics);
    other._graphics = nullptr;
    _player = std::move(other._player);
    other._player = nullptr;
    
    for(int i=0; i < other._enemies.size(); i++){
        _enemies.emplace_back(std::move(other._enemies[i]));
        other._enemies[i] = nullptr;
    }
        
    for(int i=0; i < other._threads.size(); i++){
        _threads.emplace_back(std::move(other._threads[i]));
    }
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

    sf::Event event;

    auto lastUpdate = std::chrono::system_clock::now();
    int timeSinceLastUpdate = 0;

    _isRunning = true;

    while(_graphics->GetWindow().isOpen()){

        
        timeSinceLastUpdate = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - lastUpdate).count();
        if(timeSinceLastUpdate < _graphics->GetFrameDuration()){
            sf::sleep(sf::milliseconds(_graphics->GetFrameDuration() - timeSinceLastUpdate));
        }
        
        ProcessInput(event);
        Update();
        Render();

        lastUpdate = std::chrono::system_clock::now();

    }
}

void GameManager::OnKeyPressed(sf::Event& event)
{
    switch (event.key.code)
    {
    case sf::Keyboard::Key::Space:
        if(_isPaused){
            Resume();
        } else {
            Pause();
        }
        break;
    case sf::Keyboard::Key::Up:
        if(!_isPaused) _player->SetVelocity(0, -1.5f);
        break;
    case sf::Keyboard::Key::Down:
        if(!_isPaused) _player->SetVelocity(0, 1.5f);
        break;
    case sf::Keyboard::Key::Left:
        if(!_isPaused) _player->SetVelocity(-1.5f, 0);
        break;
    case sf::Keyboard::Key::Right:
        if(!_isPaused) _player->SetVelocity(1.5f, 0);
        break;
    
    default:
        break;
    }
}

void GameManager::ProcessInput(sf::Event& event)
{
    std::cout << "Polling events..." << std::endl;
    // polling events
    _graphics->GetWindow().pollEvent(event);

    // handling input
    switch (event.type) {
        case sf::Event::Closed:
            EndGame();
            return;
        case sf::Event::KeyPressed:
            OnKeyPressed(event);
            break;
        default:
            break;
    }
}

void GameManager::Update()
{
    if(_isPaused || !_isRunning)
        return;

    // Implement Collision detection
    // Update lives and score
    // Update transform
    // Update food and powerups
    // 
    sf::Vector2f pos = _player->GetShape().getPosition();
    sf::Vector2f vel = _player->GetTransform().velocity;
    pos += vel;

    // Wrap around window border
    if(pos.x > _graphics->GetWidth()) pos.x = 0.0f;
    if(pos.x < 0.0f) pos.x = _graphics->GetWidth();
    if(pos.y > _graphics->GetHeight()) pos.y = 0.0f;
    if(pos.y < 0.0f) pos.y = _graphics->GetHeight();

    _player->GetShape().setPosition(pos);

    std::cout << "Game updated" << std::endl;
}

void GameManager::Render()
{
    if(_isPaused || !_isRunning)
        return;
    
    sf::RenderWindow& window = _graphics->GetWindow();
    window.clear();

    _graphics->Render(_player);

    std::for_each(_enemies.begin(), _enemies.end(), [this](std::unique_ptr<Entity>& enemy){
        this->_graphics->Render(enemy);
    });

    window.display();
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

void GameManager::Pause()
{
    std::cout << "Game Paused..." << std::endl;
    _isPaused = true;
}

void GameManager::Resume()
{
    std::cout << "Game Resumed..." << std::endl;
    _isPaused = false;
}

bool GameManager::InitPlayer()
{
    // create texture and set player transform in maze
    std::cout << "Player Initialized" << std::endl;

    // update position and velocity
    _player->SetPosition(static_cast<float>(_graphics->GetWidth()) / 2.0f, static_cast<float>(_graphics->GetHeight()) / 2.0f);
    _player->SetVelocity( 1.5f, 0.0f );

    // creating new thread for Player
    std::thread t(&Entity::Init, std::ref(*_player));
    // thread moved to game manager threads vector
    _threads.emplace_back(std::move(t));

    return true;
}

bool GameManager::InitEnemies()
{   
    _enemies.emplace_back(std::make_unique<Entity>(EntityType::ENEMY, EnemyTag::MAGENTA));
    _enemies.emplace_back(std::make_unique<Entity>(EntityType::ENEMY, EnemyTag::BLUE));
    _enemies.emplace_back(std::make_unique<Entity>(EntityType::ENEMY, EnemyTag::GREEN));
    _enemies.emplace_back(std::make_unique<Entity>(EntityType::ENEMY, EnemyTag::RED));

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
    _graphics->GetWindow().close();
}