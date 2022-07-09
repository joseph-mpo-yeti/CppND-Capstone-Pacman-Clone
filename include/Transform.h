#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <SFML/Graphics.hpp>

struct Transform
{   
    sf::Vector2f position;
    sf::Vector2f rotation;
    sf::Vector2f velocity;
};


#endif // TRANSFORM_H