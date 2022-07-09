#include <iostream>

#include "Entity.h"

Entity::Entity(EntityType entityType, EnemyTag tag) : _entityType(entityType), _tag(tag), _isAlive(true)
{
    _shape = sf::CircleShape(15.0f, 60UL);
    _shape.setOrigin(_shape.getRadius() / 2.0f, _shape.getRadius() / 2.0f);
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
