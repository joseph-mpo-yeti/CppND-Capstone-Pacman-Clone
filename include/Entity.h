#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>
#include <memory>

#include "Tag.h"
#include "Physics.h"

class Entity
{
    public:
        // constructor
        Entity(Tag&& tag, const std::string& filename);
        
        // getters / setters
        Tag GetTag() const;
        Transform& GetTransform();
        Collider& GetCollider();
        sf::Texture* GetTexture();

        void LoadTexture(std::unique_ptr<sf::Texture> texture);
        void SetTag(Tag tag);
        bool IsAlive();
    
    private:
        // data members
        Tag _tag;
        bool _isAlive;
        Transform _transform;
        Collider _collider;
        std::unique_ptr<sf::Texture> _texture { nullptr };

};

#endif // ENTITY_H