#ifndef COLLIDER_H
#define COLLIDER_H

#include <SFML/Graphics.hpp>

class CollisionDetector
{
    public:
        CollisionDetector(){};
        static bool AreColliding(sf::Vector2f positionPlayer, sf::Vector2f positionEnemy, float radius);

};

#endif // COLLIDER_H