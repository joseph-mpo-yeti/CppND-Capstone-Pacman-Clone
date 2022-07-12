#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Tag.h"
#include "Transform.h"
#include "Intersection.h"

class Entity
{
    public:
        // constructor
        Entity(EntityType entityType, EnemyTag tag);
        
        // getters / setters
        EntityType GetType() const { return _entityType; }
        EnemyTag GetTag() const { return _tag; }
        Transform& GetTransform() { return _transform; }
        Direction GetDirection(){ return _direction; }
        EntityState GetState() { return _state; }
        sf::RectangleShape& GetShape(){ return _shape; }
        std::vector<sf::RectangleShape>& GetShapes(Direction direction){ return _shapes.at(direction); }
        sf::Vector2u ComputeTopLeftPoint(sf::Vector2u coordinates, sf::Vector2f clipSize);
        sf::Vector2f GetSize() { return _size; }

        void SetVelocity(float x , float y){ _transform.velocity = {x ,y}; }
        void SetPosition(float x , float y){ _shape.setPosition({x ,y}); }
        void SetPosition(sf::Vector2f pos){ _shape.setPosition(pos); }
        void SetDirection(Direction dir){ _direction = dir; }
        void SetType(EntityType type){ _entityType = type; }
        void SetTag(EnemyTag tag){ _tag = tag; }
        void SetShape(sf::RectangleShape shape){ _shape = shape; }
        void LoadTexCoordinates();
        void AddPlayerTexCoordinates(sf::Vector2u coordinates[23]);
        void AddEnemyTexCoordinates(sf::Vector2u coordinates[8]);
        void AddCoordinates(std::vector<sf::Vector2u> vecUp, std::vector<sf::Vector2u> vecDown, std::vector<sf::Vector2u> vecLeft, std::vector<sf::Vector2u> vecRight);
        void LoadShapes(sf::Vector2f clipSize, sf::Texture* texture);
        void Animate();

    private:
        EntityType _entityType;
        EnemyTag _tag;
        EntityState _state;
        Transform _transform;
        Direction _direction;
        sf::Vector2f _size;
        sf::RectangleShape _shape;
        std::unordered_map<Direction, std::vector<sf::Vector2u>> _texCoordinates;
        std::unordered_map<Direction, std::vector<sf::RectangleShape>> _shapes;

};

#endif // ENTITY_H