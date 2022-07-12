
#include "Animation.h"

void Animation::Update(Entity* entity)
{
    if(!entity->_shapes.empty() && entity->_shapes.size() >= 2){
        entity->SetShape(entity->GetShapes(entity->_direction)[entity->_animationIndex]);
        entity->SetPosition(entity->_transform.position);
    }

    UpdateIndex(entity);
}

void Animation::UpdateIndex(Entity* entity)
{
    entity->_animationIndex++;

    switch (entity->GetType())
    {
        case EntityType::PLAYER:
            if(entity->_animationIndex > 2) entity->_animationIndex = 1;
            break;
        case EntityType::ENEMY:
            if(entity->_animationIndex > 1) entity->_animationIndex = 0;
            break;
        default:
            break;
    }
}