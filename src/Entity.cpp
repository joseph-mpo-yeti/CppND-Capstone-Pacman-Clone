#include <iostream>

#include "Entity.h"

Entity::Entity(EntityType entityType, EnemyTag tag) : _entityType(entityType), _tag(tag), _state(EntityState::IDLE)
{
    _radius = 15.0f;
    _shape = sf::CircleShape(_radius);
    _shape.setOrigin(_radius / 2.0f, _radius / 2.0f);
}

void Entity::LoadTexture(std::unique_ptr<sf::Texture> texture)
{
    _texture = std::move(texture);
}

void Entity::Init()
{
    switch (_tag)
    {
    case EnemyTag::PLAYER:
        _shape.setFillColor(sf::Color::Yellow);
        break;
    case EnemyTag::RED:
        _shape.setFillColor(sf::Color::Red);
        break;
    case EnemyTag::MAGENTA:
        _shape.setFillColor(sf::Color::Magenta);
        break;
    case EnemyTag::BLUE:
        _shape.setFillColor(sf::Color::Blue);
        break;
    case EnemyTag::GREEN:
        _shape.setFillColor(sf::Color::Green);
        break;
    default:
        break;
    }
}
