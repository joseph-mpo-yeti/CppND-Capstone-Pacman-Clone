#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>
#include <memory>

#include "Tag.h"
#include "Transform.h"
#include "Collider.h"

class Entity
{
    public:
        // constructor
        Entity(EntityType entityType, EnemyTag tag);
        
        // getters / setters
        EntityType GetType() const { return _entityType; };
        EnemyTag GetTag() const { return _tag; };
        Transform& GetTransform() { return _transform; };
        Collider& GetCollider() { return _collider; };
        sf::Texture& GetTexture(){ return *_texture.get(); };
        void SetVelocity(float x , float y){ _transform.velocity = {x ,y}; }
        void SetPosition(float x , float y){ _shape.setPosition({x ,y}); }
        sf::CircleShape& GetShape(){ return _shape; };
        void SetType(EntityType type){ _entityType = type; };
        void SetTag(EnemyTag tag){ _tag = tag; };
        bool IsAlive() { return _isAlive; };

        // behavior
        void Init();
        void LoadTexture(std::unique_ptr<sf::Texture> texture);
        
    
    private:
        // data members
        EntityType _entityType;
        EnemyTag _tag;
        bool _isAlive;
        Transform _transform;
        Collider _collider;
        // TODO: remove and leave texture
        sf::CircleShape _shape;
        std::unique_ptr<sf::Texture> _texture { nullptr };

};

#endif // ENTITY_H