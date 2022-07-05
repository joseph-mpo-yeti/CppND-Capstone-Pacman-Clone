#include <iostream>
#include <chrono>
#include <thread>

#include "Graphics.h"

/* Graphics class implementation */

Graphics::~Graphics()
{
    SDL_Quit();
}

bool Graphics::Init()
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "There was an erroor while initializing SDL: " << SDL_GetError() << std::endl;
        return false;
    };

    // creating window and renderer
    _window.reset(SDL_CreateWindow(_title.c_str(), 0, 0, _width, _height, SDL_WINDOW_SHOWN));
    _renderer.reset(SDL_CreateRenderer(_window.get(), -1, 0));

    SDL_Event event;

    auto lastUpdate = std::chrono::system_clock::now();

    // Rendering loop
    while(true){

        // polling events
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) {
            break;
        }

        int timeSinceLastUpdate = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - lastUpdate).count();
        if( timeSinceLastUpdate < _frameDuration)
            std::this_thread::sleep_for(std::chrono::milliseconds(_frameDuration - timeSinceLastUpdate));

        Update();
        lastUpdate = std::chrono::system_clock::now();
    }

    return true;
}

void Graphics::Update()
{
    auto renderer = getRenderer();
    SDL_RenderClear(renderer);
    SDL_Rect rect { _width / 2, _height / 2, 15, 15};
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderPresent(renderer);
}

SDL_Window* Graphics::getWindow()
{
    return _window.get();
}

SDL_Renderer* Graphics::getRenderer()
{
    return _renderer.get();
}