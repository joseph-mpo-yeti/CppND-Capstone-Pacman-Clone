
#include "Animation.h"

void Animation::Update(Entity* entity)
{   
    UpdateShapes(entity);

    if(!entity->_shapes.empty() || !entity->_stateShapes.empty()){
        switch(entity->GetType()){
            case EntityType::PLAYER:
                switch(entity->GetState()){
                    case EntityState::DEAD:
                        entity->SetShape(entity->GetShapes(entity->_state)[entity->_animationIndex]);
                        if(entity->_animationIndex == entity->_maxAnimationIndex) entity->_deadAnimationOver = true;
                        break;
                    default:
                        entity->SetShape(entity->GetShapes(entity->_direction)[entity->_animationIndex]);
                        break;
                }
                break;
            case EntityType::ENEMY:
                switch(entity->GetState()){
                    case EntityState::VULNERABLE:
                        entity->SetShape(entity->GetShapes(entity->_state)[entity->_animationIndex]);
                        break;
                    case EntityState::DEAD:
                        entity->SetShape(entity->GetShapes(entity->_state)[entity->_animationIndex]);
                        break;
                    default:
                        entity->SetShape(entity->GetShapes(entity->_direction)[entity->_animationIndex]);
                        break;
                }
                break;
            default:
                break;
        }

        entity->SetPosition(entity->_transform.position);
    }

    UpdateIndex(entity);
}

void Animation::UpdateIndex(Entity* entity)
{
    entity->_animationIndex++;
        
    if(entity->_animationIndex > entity->_maxAnimationIndex) entity->_animationIndex = entity->_minAnimationIndex;

}

void Animation::UpdateShapes(Entity* entity)
{
    try {
        if(entity->StateHasChanged()){
            auto& states = entity->GetShapes(entity->GetState());
            
            entity->SetMinAnimationIndex(0);
            entity->SetAnimationIndex(0);
            entity->SetMaxAnimationIndex(states.size()-1);

            entity->SetStateChanged(false);
        }

        entity->SetShape(entity->GetState());
        
    } catch(...){

    }

}