#include <iostream>
#include <chrono>
#include <thread>

#include <SDL2/SDL_image.h>

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
    IMG_Quit();
    SDL_Quit();
}

bool Graphics::Init()
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "There was an error initializing SDL: " << SDL_GetError() << std::endl;
        return false;
    };
    
    if(SDL_Init(IMG_INIT_PNG) < 0)
    {
        std::cout << "There was an error initializing SDL_Image: " << IMG_GetError() << std::endl;
        return false;
    };

    // creating window and renderer
    _window.reset(SDL_CreateWindow(_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _width, _height, SDL_WINDOW_SHOWN));
    _renderer.reset(SDL_CreateRenderer(_window.get(), -1, 0));

    if(_window.get() == NULL || _renderer.get() == NULL)
    {
        std::cout << "There was an error creating a window and renderer: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

int Graphics::GetFrameDuration()
{
    return _frameDuration;
}

int Graphics::GetFrameRate()
{
    return _frameRate;
}

void Graphics::Clear()
{
    SDL_RenderClear(_renderer.get());
}

// helper for printing tag
std::ostream& operator<<(std::ostream& out, const Tag& tag)
{
    switch (tag)
    {
    case Tag::FOOD:
        out << "FOOD";
        break;
    case Tag::POWER_UP:
        out << "POWER_UP";
        break;
    case Tag::PLAYER:
        out << "PLAYER";
        break;
    case Tag::ENEMY:
        out << "ENEMY";
        break;
    case Tag::WALL:
        out << "WALL";
        break;
    default:
        break;
    }

    return out;
}

void Graphics::Render(const Entity& entity){
    std::cout << "Rendering entity with tag: " << entity.GetTag() << std::endl; 
}

void Graphics::Present()
{
    SDL_RenderPresent(_renderer.get());
}


std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> Graphics::LoadTexture(std::string filename)
{
    std::string path = SDL_GetBasePath();
    path.append("data/").append(filename);
    std::cout << "Basepath is "<< path << std::endl;

    std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> surface = { 
        IMG_Load(path.c_str()), 
        [](SDL_Surface* sf){ 
            std::cout << "Surface freed" << std::endl; 
            SDL_FreeSurface(sf); 
        }
    };

    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> texture = { 
        SDL_CreateTextureFromSurface(_renderer.get(), surface.get()), 
        [](SDL_Texture* tx){ 
            std::cout << "Texture destroyed" << std::endl; 
            SDL_DestroyTexture(tx); 
        }
    } ;

    return std::move(texture);
    
}