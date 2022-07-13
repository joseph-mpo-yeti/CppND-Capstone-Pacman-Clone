#ifndef ANIMATION_H
#define ANIMATION_H

#include "Entity.h"

#include <chrono>

// forward declaration
class Animation;

class Animation
{
    public:
        static void Update(Entity* entity);
        static void UpdateIndex(Entity* entity);
        static void UpdateShapes(Entity* entity);

};

#endif // ANIMATION_H