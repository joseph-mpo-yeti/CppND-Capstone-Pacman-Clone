#ifndef PHYSICS_H
#define PHYSICS_H

#include <memory>
#include <box2d/box2d.h>


class Physics {
  
public:
    Physics();
    std::unique_ptr<b2World>& GetWorld() { return _b2World; };

private:
    std::unique_ptr<b2World> _b2World;
    
};

#endif
