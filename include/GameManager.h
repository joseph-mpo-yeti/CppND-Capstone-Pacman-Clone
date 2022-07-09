#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <memory>
#include <vector>

#include "Graphics.h"
#include "Entity.h"
#include "Assets.h"

// forward declaration
class GameManager;

class GameManager 
{
    public:
        // constructor
        GameManager();

        // bahavior
        bool Init();
        void Run();

    private:
        // bahavior
        void Update();
        void Render();
        void Pause();
        void Resume();
        void HideLoading();
        void ShowLoading();
        void ShowGame();
        void HideGame();
        void EndGame();
        void ProcessInput(sf::Event& event);
        
        bool InitPlayer();
        bool InitEnemies();
        bool InitMaze();

    private:
        // private members
        int _score { 0 };
        bool _isPaused = false;
        bool _isRunning = false;
        std::unique_ptr<Graphics> _graphics;
        std::unique_ptr<Entity> _player;
        std::vector<std::unique_ptr<Entity>> _enemies; 
};

#endif // GAME_MANAGER_H