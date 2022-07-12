#include <iostream>
#include <utility>
#include <algorithm>

#include "Entity.h"
#include "Assets.h"

Entity::Entity(EntityType entityType, EnemyTag tag) : _entityType(entityType), _tag(tag), _state(EntityState::IDLE)
{
    _size = { 35.0f, 35.0f };
    _shape = sf::RectangleShape(_size);
    _shape.setOrigin(_size.x / 2.0f, _size.y / 2.0f);
}

void Entity::LoadTexCoordinates()
{
   
    switch (_tag)
    {
        case EnemyTag::PLAYER:
            AddPlayerTexCoordinates(Assets::image::PLAYER_TEX);
            break;
        case EnemyTag::RED:
            AddEnemyTexCoordinates(Assets::image::ENEMY_RED_TEX);
            break;
        case EnemyTag::MAGENTA:
            AddEnemyTexCoordinates(Assets::image::ENEMY_MAGENTA_TEX);
            break;
        case EnemyTag::BLUE:
            AddEnemyTexCoordinates(Assets::image::ENEMY_BLUE_TEX);
            break;
        case EnemyTag::GREEN:
            AddEnemyTexCoordinates(Assets::image::ENEMY_GREEN_TEX);
            break;
        default:
            break;
    }
}

void Entity::LoadShapes(sf::Vector2f clipSize, sf::Texture* texture)
{
    std::for_each(_texCoordinates.begin(), _texCoordinates.end(), [this, &clipSize, texture](auto& coordinates){
        Direction direction = coordinates.first;
        std::for_each(coordinates.second.begin(), coordinates.second.end(), [this, &clipSize, &direction, texture](sf::Vector2u& coordinate){
            sf::Vector2u leftOrigin = ComputeTopLeftPoint(coordinate, clipSize);
            sf::RectangleShape shape(this->GetShape());
            shape.setTexture(texture);
            std::vector<sf::RectangleShape> shapes {};
            shape.setTextureRect(sf::IntRect(leftOrigin.x, leftOrigin.y, static_cast<u_int>(clipSize.x), static_cast<u_int>(clipSize.y)));
            try {
                this->_shapes.at(direction).emplace_back(shape);
            } catch(...) {
                this->_shapes[direction] = std::vector<sf::RectangleShape>();
                this->_shapes.at(direction).emplace_back(shape);
            }
            this->_shapes.insert(std::make_pair<Direction, std::vector<sf::RectangleShape>>(std::move(direction), std::move(shapes)));
        });
    });
}

sf::Vector2u Entity::ComputeTopLeftPoint(sf::Vector2u coordinates, sf::Vector2f clipSize)
{
    auto x = 1 + (coordinates.x * 15) + (coordinates.x * static_cast<u_int>(clipSize.x));
    auto y = 4 + (coordinates.y * 15) + (coordinates.y * static_cast<u_int>(clipSize.y));
    return sf::Vector2u(x, y);
}

void Entity::AddCoordinates(std::vector<sf::Vector2u> vecUp, std::vector<sf::Vector2u> vecDown, std::vector<sf::Vector2u> vecLeft, std::vector<sf::Vector2u> vecRight)
{
    _texCoordinates.insert(std::make_pair<Direction, std::vector<sf::Vector2u>>(Direction::UP, std::move(vecUp)));
    _texCoordinates.insert(std::make_pair<Direction, std::vector<sf::Vector2u>>(Direction::DOWN, std::move(vecDown)));
    _texCoordinates.insert(std::make_pair<Direction, std::vector<sf::Vector2u>>(Direction::LEFT, std::move(vecLeft)));
    _texCoordinates.insert(std::make_pair<Direction, std::vector<sf::Vector2u>>(Direction::RIGHT, std::move(vecRight)));
}

void Entity::AddPlayerTexCoordinates(sf::Vector2u* coordinates)
{
    AddCoordinates(
        {coordinates[0], coordinates[1], coordinates[2]},
        {coordinates[3], coordinates[4], coordinates[5]},
        {coordinates[6], coordinates[7], coordinates[8]},
        {coordinates[9], coordinates[10], coordinates[11]}
    );
}

void Entity::AddEnemyTexCoordinates(sf::Vector2u* coordinates)
{
    AddCoordinates(
        {coordinates[6], coordinates[7]}, 
        {coordinates[2], coordinates[3]}, 
        {coordinates[4], coordinates[5]}, 
        {coordinates[0], coordinates[1]}
    );
}

std::string Entity::GetTagName()
{
    switch (_tag)
    {
    case EnemyTag::PLAYER:
        return "PLAYER";
        break;
    case EnemyTag::RED:
        return "ENEMY_RED";
        break;
    case EnemyTag::GREEN:
        return "ENEMY_GREEN";
        break;
    case EnemyTag::MAGENTA:
        return "ENEMY_MAGENTA";
        break;
    case EnemyTag::BLUE:
        return "ENEMY_BLUE";
        break;
    default:
        break;
    }
}