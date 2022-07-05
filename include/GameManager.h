#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <thread>
#include <vector>

#include "Graphics.h"

class GameManager 
{
    public:
        // constructor / destructor
        GameManager();
        ~GameManager();

        // getters / setters

        // bahavior
        void run();

    private:
        // behavior

    private:
        // private members
        std::unique_ptr<Graphics> _graphics = nullptr;
};

#endif // GAME_MANAGER_H