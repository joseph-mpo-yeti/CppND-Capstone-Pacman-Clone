#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H


enum class Direction 
{
    Up, Down, Left, Right
};

class GameController
{
    public:
        GameController();
        ~GameController();

    public:
        void Setup();
        void Update();
        void Pause();
        void Resume();

    private:

};

#endif // GAME_CONTROLLER_H