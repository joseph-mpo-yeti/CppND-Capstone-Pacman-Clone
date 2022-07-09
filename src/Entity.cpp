#include <iostream>

#include "Entity.h"

Entity::Entity(Tag&& tag, const std::string& filename) : _tag(tag), _isAlive(true)
{
    
}


Tag Entity::GetTag() const
{
    return _tag;
}

Transform& Entity::GetTransform()
{
    return _transform;
}

Collider& Entity::GetCollider()
{
    return _collider;
}

SDL_Texture* Entity::GetTexture()
{
    return _texture.get();
}

void Entity::LoadTexture(std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> texture)
{
    _texture = std::move(texture);
}

void Entity::SetTag(Tag tag)
{
    _tag = tag;
}

bool Entity::IsAlive()
{
    return _isAlive;
}