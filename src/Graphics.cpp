#include <iostream>
#include <chrono>
#include <thread>

#include "Graphics.h"

/* Graphics class implementation */
Graphics::Graphics(): _title("Pacman"), _width(700.0f), _aspectRatio(4 / 3),  _frameRate(60)
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
    _window->setFramerateLimit(60);

    return true;
}

// helper for printing tag
std::ostream& operator<<(std::ostream& out, const EnemyTag& tag)
{
    switch (tag)
    {
    case EnemyTag::PLAYER:
        out << "PLAYER";
        break;
    case EnemyTag::RED:
        out << "ENEMY_RED";
        break;
    case EnemyTag::GREEN:
        out << "ENEMY_GREEN";
        break;
    case EnemyTag::MAGENTA:
        out << "ENEMY_MAGENTA";
        break;
    case EnemyTag::BLUE:
        out << "ENEMY_BLUE";
        break;
    default:
        break;
    }

    return out;
}

void Graphics::Render(std::unique_ptr<Entity>& entity){
    std::cout << "Rendering entity with tag: " << entity->GetTag() << std::endl; 
    _window->draw(entity->GetShape());
    
}

std::unique_ptr<sf::Texture> Graphics::LoadTexture(std::string filename)
{
    std::unique_ptr<sf::Texture> tex = std::make_unique<sf::Texture>();

    return std::move(tex);
}
