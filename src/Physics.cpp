#include <box2d/box2d.h>

#include "Physics.h"



Physics::Physics()
{
    _b2World = std::make_unique<b2World>(b2Vec2{0.0f, -10.0f});
}
