#include <cmath>

#include "Collision.h"
#include "Transform.h"
#include "Entity.h"

bool CollisionDetector::AreColliding(sf::Vector2f positionPlayer, sf::Vector2f positionEnemy, sf::Vector2f size)
{
    int distance = sqrt( pow(positionPlayer.x - positionEnemy.x, 2) + powf(positionPlayer.y - positionEnemy.y, 2));
    
    if( distance < static_cast<int>(size.x - 3) )
        return true;

    return false;
}