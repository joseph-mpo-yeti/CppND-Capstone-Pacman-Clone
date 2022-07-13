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
            AddStateTexCoordinates(EntityState::WAITING, { Assets::image::PLAYER_TEX[12] });
            AddStateTexCoordinates(EntityState::IDLE, { 
                Assets::image::PLAYER_TEX[13], Assets::image::PLAYER_TEX[14], Assets::image::PLAYER_TEX[15], 
                Assets::image::PLAYER_TEX[16] 
            });
            AddStateTexCoordinates(EntityState::DEAD, {
                Assets::image::PLAYER_TEX[17], Assets::image::PLAYER_TEX[18], Assets::image::PLAYER_TEX[19],
                Assets::image::PLAYER_TEX[20], Assets::image::PLAYER_TEX[21], Assets::image::PLAYER_TEX[22],
                Assets::image::PLAYER_TEX[23], Assets::image::PLAYER_TEX[24], Assets::image::PLAYER_TEX[25],
                Assets::image::PLAYER_TEX[26], Assets::image::PLAYER_TEX[27], Assets::image::PLAYER_TEX[28]
            });
            break;
        case EnemyTag::RED:
            AddEnemyTexCoordinates(Assets::image::ENEMY_RED_TEX);
            AddEnemyStateTexCoordinates();
            break;
        case EnemyTag::MAGENTA:
            AddEnemyTexCoordinates(Assets::image::ENEMY_MAGENTA_TEX);
            AddEnemyStateTexCoordinates();
            break;
        case EnemyTag::BLUE:
            AddEnemyTexCoordinates(Assets::image::ENEMY_BLUE_TEX);
            AddEnemyStateTexCoordinates();
            break;
        case EnemyTag::GREEN:
            AddEnemyTexCoordinates(Assets::image::ENEMY_GREEN_TEX);
            AddEnemyStateTexCoordinates();
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

    std::for_each(_stateTexCoordinates.begin(), _stateTexCoordinates.end(), [this, &clipSize, texture](auto& coordinates){
        EntityState state = coordinates.first;
        std::for_each(coordinates.second.begin(), coordinates.second.end(), [this, &clipSize, &state, texture](sf::Vector2u& coordinate){
            sf::Vector2u leftOrigin = ComputeTopLeftPoint(coordinate, clipSize);
            sf::RectangleShape shape(this->GetShape());
            shape.setTexture(texture);
            std::vector<sf::RectangleShape> shapes {};
            shape.setTextureRect(sf::IntRect(leftOrigin.x, leftOrigin.y, static_cast<u_int>(clipSize.x), static_cast<u_int>(clipSize.y)));
            try {
                this->_stateShapes.at(state).emplace_back(shape);
            } catch(...) {
                this->_stateShapes[state] = std::vector<sf::RectangleShape>();
                this->_stateShapes.at(state).emplace_back(shape);
            }
            this->_stateShapes.insert(std::make_pair<EntityState, std::vector<sf::RectangleShape>>(std::move(state), std::move(shapes)));
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

void Entity::AddStateTexCoordinates(EntityState state, std::vector<sf::Vector2u> vec)
{
    _stateTexCoordinates.insert(std::make_pair<EntityState, std::vector<sf::Vector2u>>(std::move(state), std::move(vec)));
}

void Entity::AddEnemyStateTexCoordinates()
{
    AddStateTexCoordinates(EntityState::DEAD, {
        Assets::image::ENEMY_RED_TEX[8], Assets::image::ENEMY_RED_TEX[9], Assets::image::ENEMY_RED_TEX[10], Assets::image::ENEMY_RED_TEX[11]
    });
    AddStateTexCoordinates(EntityState::VULNERABLE, { Assets::image::ENEMY_RED_TEX[12], Assets::image::ENEMY_RED_TEX[13] });
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