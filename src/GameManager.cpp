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
    _state = other._state;
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

    LoadTexture();

    // loading assets in separate threads
    auto mazeInitFuture = std::async(std::launch::async, &GameManager::InitMaze, this);
    auto enemiesInitFuture = std::async(std::launch::async, &GameManager::InitEnemies, this);
    auto playerInitFuture = std::async(std::launch::async, &GameManager::InitPlayer, this);

    if(!playerInitFuture.get() || !enemiesInitFuture.get() || !mazeInitFuture.get()){
        std::cout << "Failed to initialize game objects. " << std::endl;
        return false;
    }

    HideLoading();

    return true;
}

void GameManager::LoadTexture()
{
    _spriteSheetTexture.loadFromFile("../assets/images/sprite_sheet.png");
}

void GameManager::Run()
{
    HideLoading();
    
    ShowGame();

    sf::Event event;

    auto lastUpdate = std::chrono::system_clock::now();
    int timeSinceLastUpdate = 0;
    auto lastPlayerAnimationUpdate = std::chrono::system_clock::now();
    auto lastEnemiesAnimationUpdate = std::chrono::system_clock::now();

    _state = GameState::RUNNING;

    while(_graphics->GetWindow().isOpen()){
        timeSinceLastUpdate = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - lastUpdate).count();
        
        if(timeSinceLastUpdate < _graphics->GetFrameDuration()){
            sf::sleep(sf::milliseconds(_graphics->GetFrameDuration() - timeSinceLastUpdate));
        }

        ProcessInput(event);
        Update(lastPlayerAnimationUpdate, lastEnemiesAnimationUpdate);
        Render();

        lastUpdate = std::chrono::system_clock::now();

    }
}

void GameManager::OnKeyPressed(sf::Event& event)
{
    if(event.key.code == sf::Keyboard::Key::Space){
        if(_state == GameState::PAUSED){
            Resume();
        } else {
            Pause();
        }

        return;
    }


    if(_state == GameState::RUNNING)
    {
        switch (event.key.code)
        {
            case sf::Keyboard::Key::Up:
                _player->SetDirection(Direction::UP);
                _player->SetVelocity(0, -1.5f);
                break;
            case sf::Keyboard::Key::Down:
                _player->SetDirection(Direction::DOWN);
                _player->SetVelocity(0, 1.5f);
                break;
            case sf::Keyboard::Key::Left:
                _player->SetDirection(Direction::LEFT);
                _player->SetVelocity(-1.5f, 0);
                break;
            case sf::Keyboard::Key::Right:
                _player->SetDirection(Direction::RIGHT);
                _player->SetVelocity(1.5f, 0);
                break;
            
            default:
                break;
        }
    }
}

void GameManager::ProcessInput(sf::Event& event)
{
    if(_state != GameState::PAUSED)
        std::cout << "Polling events..." << std::endl;
    // polling events
    _graphics->GetWindow().pollEvent(event);
    int res;

    // handling input
    switch (event.type) {
        case sf::Event::Closed:
            EndGame();
            break;
        case sf::Event::KeyPressed:
            OnKeyPressed(event);
            break;
        default:
            break;
    }
}

void GameManager::Update(std::chrono::system_clock::time_point& lastPlayerAnimationUpdate, std::chrono::system_clock::time_point& lastEnemiesAnimationUpdate)
{
    if(_state != GameState::RUNNING)
        return;

    int lastPlayerUpdateDuration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - lastPlayerAnimationUpdate).count();
    int lastEnemiesUpdateDuration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - lastEnemiesAnimationUpdate).count();

    // Update enemy positions
    std::for_each(_enemies.begin(), _enemies.end(), [this, lastEnemiesUpdateDuration, &lastEnemiesAnimationUpdate](std::unique_ptr<Entity>& enemy){
        UpdateEntityPosition(enemy);
        if( lastEnemiesUpdateDuration >= 250){
            Animation::Update(enemy.get());
            lastEnemiesAnimationUpdate = std::chrono::system_clock::now();
        }
    });

    // Update player position
    UpdateEntityPosition(_player);

    if( lastPlayerUpdateDuration >= 100){
        Animation::Update(_player.get());
        lastPlayerAnimationUpdate = std::chrono::system_clock::now();
        std::cout << "Updated Animation" << std::endl;
    }

    // detect collision
    std::for_each(_enemies.begin(), _enemies.end(), [this](std::unique_ptr<Entity>& enemy){
        if(CollisionDetector::AreColliding(_player->GetTransform().position, enemy->GetTransform().position, enemy->GetSize())){
            std::cout << "Player collided with an enemy" << std::endl;
            _player->SetState(EntityState::DEAD);
            Pause();
        }
    });

    std::cout << "Game updated" << std::endl;
}

void GameManager::UpdateEntityPosition(std::unique_ptr<Entity>& entity)
{
    sf::Vector2f pos = entity->GetShape().getPosition();
    sf::Vector2f vel = entity->GetTransform().velocity;
    pos += vel;


    // Wrap entity around window border
    if(pos.x > _graphics->GetWidth()) pos.x = 0.0f;
    if(pos.x < 0.0f) pos.x = _graphics->GetWidth();
    if(pos.y > _graphics->GetHeight()) pos.y = 0.0f;
    if(pos.y < 0.0f) pos.y = _graphics->GetHeight();

    entity->GetTransform().position = pos;
    entity->GetShape().setPosition(pos);
}

void GameManager::UpdateEntityTexture(std::unique_ptr<Entity>& entity)
{
    auto shapes = entity->GetShapes(entity->GetDirection());
    if(!shapes.empty() && shapes.size() >= 2){
        entity->SetShape(shapes[1]);
        entity->SetPosition(entity->GetTransform().position);
    }
}

void GameManager::Render()
{
    if(_state != GameState::RUNNING)
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
    _state = GameState::PAUSED;
}

void GameManager::Resume()
{
    std::cout << "Game Resumed..." << std::endl;
    _state = GameState::RUNNING;
}

bool GameManager::InitPlayer()
{
    return InitEntity(_player, _intersections[2].GetPosition());
}

bool GameManager::InitEnemies()
{   
    auto enemy_magenta = std::make_unique<Entity>(EntityType::ENEMY, EnemyTag::MAGENTA);
    auto enemy_red = std::make_unique<Entity>(EntityType::ENEMY, EnemyTag::RED);
    auto enemy_blue = std::make_unique<Entity>(EntityType::ENEMY, EnemyTag::BLUE);
    auto enemy_green = std::make_unique<Entity>(EntityType::ENEMY, EnemyTag::GREEN);

    InitEntity(enemy_magenta, _intersections[1].GetPosition());
    InitEntity(enemy_red, _intersections[0].GetPosition());
    InitEntity(enemy_blue, _intersections[3].GetPosition());
    InitEntity(enemy_green, _intersections[4].GetPosition());

    return true;
}

bool GameManager::InitEntity(std::unique_ptr<Entity>& entity, sf::Vector2f pos)
{
    entity->SetPosition(pos.x, pos.y);
    entity->SetVelocity( -1.0f, 0.0f );
    entity->SetDirection(Direction::LEFT);
    entity->SetState(EntityState::IDLE);
    entity->LoadTexCoordinates();
    entity->LoadShapes(entity->GetSize(), &_spriteSheetTexture);

    if(entity->GetType() == EntityType::ENEMY){
        _enemies.emplace_back(std::move(entity));
    }
    std::cout << entity << " Initialized." << std::endl;

    return true;
}

bool GameManager::InitMaze()
{
    _intersections.emplace_back(Intersection(100.0f, 100.0f));
    _intersections.emplace_back(Intersection(400.0f, 400));
    _intersections.emplace_back(Intersection(200.0f, 200));
    _intersections.emplace_back(Intersection(100.0f, 400));
    _intersections.emplace_back(Intersection(400.0f, 100));

    std::for_each(_intersections.begin(), _intersections.end(), [this](Intersection& inter_){
        inter_.FindNeighbors();
    });

    std::cout << "Maze Initialized" << std::endl;
    return true;
}

void GameManager::EndGame()
{
    _state = GameState::ENDED;
    _graphics->GetWindow().close();
}