#ifndef INTERSECTION_H
#define INTERSECTION_H

#include <SFML/Graphics.hpp>
#include <vector>

enum class Direction
{
    UP, DOWN, LEFT, RIGHT
};

class Intersection
{
    public:
        // constructor
        Intersection(float x, float y): _position(sf::Vector2f(x, y)){}

        // getters / setters
        sf::Vector2f GetPosition(){ return _position; }
        Intersection GetNeighbor(Direction dir);
        
        // behavior
        void FindNeighbors();
    
    private:
        // members
        std::vector<Intersection> _neighbors;
        sf::Vector2f _position;
};

#endif // INTERSECTION_H