#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>
#include <memory>

#include "Tag.h"
#include "Transform.h"
#include "Intersection.h"

enum class EntityState
{
    IDLE,
    ALIVE,
    DEAD
};

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
        sf::Texture& GetTexture(){ return *_texture.get(); }
        sf::CircleShape& GetShape(){ return _shape; }
        float GetRadius() { return _radius; }

        void SetVelocity(float x , float y){ _transform.velocity = {x ,y}; }
        void SetPosition(float x , float y){ _shape.setPosition({x ,y}); }
        void SetDirection(Direction dir){ _direction = dir; }
        void SetType(EntityType type){ _entityType = type; }
        void SetTag(EnemyTag tag){ _tag = tag; }
        void LoadTexture(std::unique_ptr<sf::Texture> texture);

        // behavior
        void Init();
    
    private:
        // data members
        EntityType _entityType;
        EnemyTag _tag;
        EntityState _state;
        Transform _transform;
        Direction _direction;
        float _radius;
        sf::CircleShape _shape;
        std::unique_ptr<sf::Texture> _texture { nullptr };

};

#endif // ENTITY_H