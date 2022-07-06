#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL_rect.h>

#include "Entity.h"
#include "Physics.h"

class Player: public Entity
{
    public:

    private:

    private:
        SDL_Rect rect;
        Velocity velocity;
        int speed;
};

#endif // PLAYER_H