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


// forward declaration
class Animation;

class Entity
{
    public:
        // constructor
        Entity(EntityType entityType, EnemyTag tag);
        friend class Animation;
        
        // getters / setters
        EntityType GetType() const { return _entityType; }
        EnemyTag GetTag() const { return _tag; }
        Transform& GetTransform() { return _transform; }
        Direction GetDirection(){ return _direction; }
        EntityState GetState() { return _state; }
        sf::RectangleShape& GetShape(){ return _shape; }
        std::vector<sf::RectangleShape>& GetShapes(Direction direction){ return _shapes.at(direction); }
        std::vector<sf::RectangleShape>& GetShapes(EntityState state){ return _stateShapes.at(state); }
        sf::Vector2u ComputeTopLeftPoint(sf::Vector2u coordinates, sf::Vector2f clipSize);
        sf::Vector2f GetSize() { return _size; }
        std::string GetTagName();
        int GetAnimationIndex(){ return _animationIndex; }
        int GetMinAnimationIndex() { return _minAnimationIndex; }
        int GetMaxAnimationIndex() { return _maxAnimationIndex; }
        bool StateHasChanged(){ return _lastStateChanged; }
        bool DeadAnimationIsOver(){ return _deadAnimationOver; }

        void SetVelocity(float x , float y){ _transform.velocity = {x ,y}; }
        void SetPosition(float x , float y){ _shape.setPosition({x ,y}); }
        void SetPosition(sf::Vector2f pos){ _shape.setPosition(pos); }
        void SetDirection(Direction dir){ _direction = dir; }
        void SetType(EntityType type){ _entityType = type; }
        void SetTag(EnemyTag tag){ _tag = tag; }
        
        void SetShape(sf::RectangleShape shape){ _shape = shape; }
        void SetShape(EntityState state){ SetShape(GetShapes(state)[_minAnimationIndex]); }
        void SetShape(Direction direction){ SetShape(GetShapes(direction)[_minAnimationIndex]); }
        void LoadShapes(sf::Vector2f clipSize, sf::Texture* texture);
        
        void SetState(EntityState state){ _state = state; }
        void LoadTexCoordinates();
        void AddPlayerTexCoordinates(sf::Vector2u coordinates[23]);
        void AddEnemyTexCoordinates(sf::Vector2u coordinates[8]);
        void AddCoordinates(std::vector<sf::Vector2u> vecUp, std::vector<sf::Vector2u> vecDown, std::vector<sf::Vector2u> vecLeft, std::vector<sf::Vector2u> vecRight);
        void AddStateTexCoordinates(EntityState state, std::vector<sf::Vector2u> vec);
        void AddEnemyStateTexCoordinates();

        void SetMinAnimationIndex(int index) { _minAnimationIndex = index; }
        void SetMaxAnimationIndex(int index) { _maxAnimationIndex = index; }
        void SetAnimationIndex(int index) { _animationIndex = index; }
        void SetStateChanged(bool hasChanged) { _lastStateChanged = hasChanged; }
        void SetDeadAnimationOver(bool isOver) { _deadAnimationOver = isOver; }

    private:
        int _animationIndex { 0 };
        int _minAnimationIndex { 0 };
        int _maxAnimationIndex { 0 };
        bool _lastStateChanged = false;
        bool _deadAnimationOver = false;
        EntityType _entityType;
        EnemyTag _tag;
        EntityState _state;
        Transform _transform;
        Direction _direction;
        sf::Vector2f _size;
        sf::RectangleShape _shape;
        std::unordered_map<Direction, std::vector<sf::Vector2u>> _texCoordinates;
        std::unordered_map<Direction, std::vector<sf::RectangleShape>> _shapes;
        std::unordered_map<EntityState, std::vector<sf::Vector2u>> _stateTexCoordinates;
        std::unordered_map<EntityState, std::vector<sf::RectangleShape>> _stateShapes;

};

#endif // ENTITY_H