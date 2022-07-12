#ifndef TAG_H
#define TAG_H

#include <ostream>

enum class EntityType
{
    FOOD,
    POWER_UP,
    PLAYER,
    ENEMY,
    WALL
};


enum class EnemyTag
{
    PLAYER, 
    RED, 
    GREEN, 
    BLUE, 
    MAGENTA
};

enum class EntityState
{
    IDLE,
    WAITING,
    ALIVE,
    VULNERABLE,
    DEAD
};

#endif // TAG_H