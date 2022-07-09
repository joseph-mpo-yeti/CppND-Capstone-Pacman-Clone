#ifndef ENTITY_H
#define ENTITY_H

#include <SDL2/SDL.h>
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
        SDL_Texture* GetTexture();

        void LoadTexture(std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> texture);
        void SetTag(Tag tag);
        bool IsAlive();
    
    private:
        // data members
        Tag _tag;
        bool _isAlive;
        Transform _transform;
        Collider _collider;
        std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> _texture { NULL, SDL_DestroyTexture };

};

#endif // ENTITY_H