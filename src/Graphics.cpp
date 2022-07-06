#include <iostream>
#include <chrono>
#include <thread>

#include "Graphics.h"
#include "Physics.h"
#include "GameController.h"

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

// TODO: move into controller/player/entity
void Graphics::InitPlayer()
{
    _rect = { (int) (_width / 2), (int) (_height / 2), 15, 15};
    _velocity = { 1, 0, 0};
    _speed = 2;
}

Graphics::~Graphics()
{
    SDL_Quit();
}

Graphics::Graphics(Graphics&& other)
{
    _renderer = std::move(other._renderer);
    _window = std::move(other._window);
    other._renderer = nullptr;
    other._window = nullptr;
}

Graphics& Graphics::operator=(Graphics&& other)
{
    if(this == &other)
        return *this;

    _renderer = std::move(other._renderer);
    _window = std::move(other._window);
    other._renderer = nullptr;
    other._window = nullptr;

    return *this;
}

bool Graphics::Init()
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "There was an error initializing SDL: " << SDL_GetError() << std::endl;
        return false;
    };

    // creating window and renderer
    _window.reset(SDL_CreateWindow(_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _width, _height, SDL_WINDOW_SHOWN));
    _renderer.reset(SDL_CreateRenderer(_window.get(), -1, 0));

    if(!_window.get() || !_renderer.get())
    {
        std::cerr << "There was an error creating a window and renderer: " << SDL_GetError() << std::endl;
        return false;
    }


    InitPlayer();

    return true;
}

void Graphics::Update()
{
    SDL_Event event;
    
    auto lastUpdate = std::chrono::system_clock::now();

    while(true){

        // polling events
        SDL_PollEvent(&event);

        // handling input
        // TODO: move direction handling into controller
        switch (event.type) {
            case SDL_QUIT:
                return;

            case SDL_KEYDOWN:
                switch (event.key.keysym.sym){
                    case SDLK_SPACE:
                        if(_isPaused) 
                            Resume();
                        else 
                            Pause();
                        break;
                    case SDLK_UP:
                        if(!_isPaused) _velocity = { 0, -1, 0 };
                        break;
                    case SDLK_DOWN:
                        if(!_isPaused) _velocity = { 0, 1, 0 };
                        break;
                    case SDLK_LEFT:
                        if(!_isPaused) _velocity = { -1, 0, 0 };
                        break;
                    case SDLK_RIGHT:
                        if(!_isPaused) _velocity = { 1, 0, 0 };
                        break;
                    default:
                        break;
                }
            // -----------------------------
            default:
                break;
        }

        int timeSinceLastUpdate = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - lastUpdate).count();
        if( timeSinceLastUpdate < _frameDuration)
            std::this_thread::sleep_for(std::chrono::milliseconds(_frameDuration - timeSinceLastUpdate));
        
        // skip updates if paused
        if(_isPaused)
            continue;

        // Update position
        _rect.x += _speed * _velocity.x;
        _rect.y += _speed * _velocity.y;

        // Wrapping around screen
        if(_rect.x > _width)
            _rect.x = 0;
        if(_rect.x < 0)
            _rect.x = _width;
        if(_rect.y > _height)
            _rect.y = 0;
        if(_rect.y < 0)
            _rect.y = _height;

        auto renderer = getRenderer();
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &_rect);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderPresent(renderer);

        lastUpdate = std::chrono::system_clock::now();
    }

}

void Graphics::Pause()
{
    _isPaused = true;
}

void Graphics::Resume()
{
    _isPaused = false;
}

SDL_Window* Graphics::getWindow()
{
    return _window.get();
}

SDL_Renderer* Graphics::getRenderer()
{
    return _renderer.get();
}