#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <memory>
#include <vector>
#include <thread>
#include <chrono>
#include <box2d/box2d.h>

#include "Graphics.h"
#include "Entity.h"
#include "Assets.h"
#include "Collision.h"
#include "Animation.h"

// forward declaration
class GameManager;

enum class GameState
{
    IDLE,
    PAUSED,
    RUNNING,
    ENDED
};

class GameManager 
{
    public:
        // constructor / destructor
        GameManager();
        ~GameManager();

        // no copy policy
        GameManager(const GameManager& other)=delete;
        GameManager& operator=(const GameManager& other)=delete;

        // move constructor / assignment operator
        GameManager(GameManager&& other);
        GameManager& operator=(GameManager&& other);

        // bahavior
        bool Init();
        void Run();

    private:
        // bahavior
        void Render();
        void Pause();
        void Resume();
        void HideLoading();
        void ShowLoading();
        void ShowGame();
        void HideGame();
        void EndGame();
        void LoadTexture();
        void ProcessInput(sf::Event& event);
        void OnKeyPressed(sf::Event& event);
        void UpdateEntityPosition(std::unique_ptr<Entity>& entity);
        void UpdateEntityTexture(std::unique_ptr<Entity>& entity);
        void Update(std::chrono::system_clock::time_point& lastPlayerAnimationUpdate, std::chrono::system_clock::time_point& lastEnemiesAnimationUpdate);

        // helpers
        void MoveHere(GameManager&& other);
        
        bool InitPlayer();
        bool InitEnemies();
        bool InitMaze();
        bool InitEntity(std::unique_ptr<Entity>& entity, sf::Vector2f pos);

    private:
        // private members
        int _score { 0 };
        GameState _state;
        CollisionDetector _collisionDetector;
        std::unique_ptr<Graphics> _graphics;
        std::unique_ptr<Entity> _player;
        std::vector<std::unique_ptr<Entity>> _enemies; 
        sf::Texture _spriteSheetTexture;
        std::vector<std::thread> _threads;
        std::vector<Intersection> _intersections;

};

#endif // GAME_MANAGER_H
