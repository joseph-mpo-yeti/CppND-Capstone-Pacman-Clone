#include <iostream>
#include <chrono>
#include <thread>

#include "Graphics.h"

/* Graphics class implementation */
Graphics::Graphics(): _title("Pacman"), _width(700.0f), _aspectRatio(4 / 3),  _frameRate(120)
{
    _height = _width * _aspectRatio;
    _frameDuration = 1000 / _frameRate;
}

Graphics::Graphics(std::string title, float width, float height, float aspectRatio, int frameRate) 
   : _title(title), _width(width), _height(height), _aspectRatio(aspectRatio),  _frameRate(frameRate) 
{
    _frameDuration = 1000 / _frameRate;
}

Graphics::~Graphics()
{
    
}

bool Graphics::Init()
{
    _window = std::make_unique<sf::RenderWindow>(sf::VideoMode(_width, _height), _title, sf::Style::Close);
    _window->setFramerateLimit(120);

    return true;
}

void Graphics::Render(std::unique_ptr<Entity>& entity){
    std::cout << "Rendering entity with tag: " << entity->GetTagName() << std::endl;
    _window->draw(entity->GetShape());
}
