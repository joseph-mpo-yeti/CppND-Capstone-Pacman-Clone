#include <iostream>
#include <box2d/box2d.h>

#include "GameManager.h"

int main()
{   
    GameManager gm;
    
    if(!gm.Init()){
        std::cout << "Game initialization failed..." << std::endl; 
        exit(1);
    }

    gm.Run();

    std::cout << "Game execution ended..." << std::endl;
    
    return 0;
}