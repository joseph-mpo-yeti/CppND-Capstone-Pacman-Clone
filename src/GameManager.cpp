#include <iostream>
#include <future>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <algorithm>
#include <chrono>
#include <exception>
#include <cmath>

#include <box2d/box2d.h>
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
    _player = std::move(other._player);
    other._graphics = nullptr;
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

    InitMaze();
    
    // loading assets in separate threads
    auto enemiesInitFuture = std::async(std::launch::async, &GameManager::InitEnemies, this);
    auto playerInitFuture = std::async(std::launch::async, &GameManager::InitPlayer, this);

    enemiesInitFuture.wait();
    playerInitFuture.wait();

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
    auto lastPlayerAnimationUpdate = lastUpdate;
    auto lastEnemiesAnimationUpdate = lastUpdate;
    int timeSinceLastUpdate = 0;

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
        if(_player->GetState() != EntityState::DEAD) {
            UpdateEntityPosition(enemy);
            if( lastEnemiesUpdateDuration >= 300){
                Animation::Update(enemy.get());
                lastEnemiesAnimationUpdate = std::chrono::system_clock::now();
            }
        }
    });

    // Update player position
    UpdateEntityPosition(_player);

    if( lastPlayerUpdateDuration >= 100){
        Animation::Update(_player.get());
        lastPlayerAnimationUpdate = std::chrono::system_clock::now();
        std::cout << "Updated Animation" << std::endl;
    }

    if(_player->GetState() != EntityState::DEAD) {
        // detect collision if player is alive
        std::for_each(_enemies.begin(), _enemies.end(), [this](std::unique_ptr<Entity>& enemy){
            if(CollisionDetector::AreColliding(_player->GetTransform().position, enemy->GetTransform().position, enemy->GetSize())){
                std::cout << "Player collided with an enemy" << std::endl;
                if(enemy->GetState() == EntityState::VULNERABLE){
                    enemy->SetState(EntityState::DEAD);
                    enemy->SetStateChanged(true);
                } else {
                    _player->SetState(EntityState::DEAD);
                    _player->SetStateChanged(true);
                }
                //Pause();
            }
        });
    }

    std::cout << "Game updated" << std::endl;
}

void GameManager::UpdateEntityPosition(std::unique_ptr<Entity>& entity)
{
    sf::Vector2f pos = entity->GetShape().getPosition();
    sf::Vector2f vel = entity->GetTransform().velocity;
    
    switch (entity->GetDirection())
    {
    case Direction::UP:
        vel = { 0, -abs(vel.y) };
        break;
    case Direction::LEFT:
        vel = { -abs(vel.x), 0 };
        break;
    case Direction::RIGHT:
        vel = { abs(vel.x), 0 };
        break;
    case Direction::DOWN:
        vel = { 0, abs(vel.y) };
        break;
    
    default:
        break;
    }

    pos += vel;


    // Wrap entity around window border if player and turn back if enemy
    if(entity->GetType() == EntityType::PLAYER){
        if(pos.x > _graphics->GetWidth()) pos.x = 0.0f;
        if(pos.x < 0.0f) pos.x = _graphics->GetWidth();
        if(pos.y > _graphics->GetHeight()) pos.y = 0.0f;
        if(pos.y < 0.0f) pos.y = _graphics->GetHeight();
    } else {
        auto half_side = entity->GetSize().x/2;
        if(pos.x + half_side > _graphics->GetWidth()) entity->SetDirection(Direction::LEFT);
        if(pos.x - half_side < 0.0f) entity->SetDirection(Direction::RIGHT);
        if(pos.y + half_side > _graphics->GetHeight()) entity->SetDirection(Direction::UP);
        if(pos.y - half_side < 0.0f) entity->SetDirection(Direction::DOWN);
    }
    if( entity->GetType() == EntityType::PLAYER && entity->GetState() == EntityState::DEAD){
        if(entity->DeadAnimationIsOver()) {
            // entity->();
            Pause();
        }
    } else {
        entity->GetTransform().position = pos;
        entity->GetShape().setPosition(pos);
    }
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

    sf::Texture texture;
    texture.loadFromFile("../assets/images/maze_sprite_sheet.png");
    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setPosition(0, 0);
    _graphics->GetWindow().draw(sprite);

    _graphics->Render(_player);

    _graphics->Render(_player);

    std::for_each(_enemies.begin(), _enemies.end(), [this](std::unique_ptr<Entity>& enemy){
        this->_graphics->Render(enemy);
    });

    window.display();
}

void GameManager::ShowGame()
{
    _graphics->Init();
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
    entity->SetVelocity( 1.0f, 0.0f );
    entity->SetDirection(Direction::RIGHT);
    entity->SetState(EntityState::IDLE);
    entity->LoadTexCoordinates();
    entity->LoadShapes(entity->GetSize(), &_spriteSheetTexture);


    /*
    b2BodyDef bodyDef;
    bodyDef.type = b2BodyType::b2_staticBody;
    bodyDef.position.Set(_transform.position.x, _transform.position.y);
    bodyDef.angle = _transform.rotation.y;
    */
    std::cout << entity->GetTagName() << " Initialized." << std::endl;

    if(entity->GetType() == EntityType::ENEMY){
        entity->SetMinAnimationIndex(0);
        entity->SetAnimationIndex(0);
        entity->SetMaxAnimationIndex(1);
        _enemies.emplace_back(std::move(entity));
    } else {
        entity->SetMinAnimationIndex(1);
        entity->SetAnimationIndex(1);
        entity->SetMaxAnimationIndex(2);
    }

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

    std::cout << "MAZE Initialized" << std::endl;
    return true;
}

void GameManager::EndGame()
{
    _state = GameState::ENDED;
    _graphics->GetWindow().close();
}
