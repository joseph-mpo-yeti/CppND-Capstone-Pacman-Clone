#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "SDL.h"

class Graphics
{

    public:
        // constructor / destructor
        Graphics(){};
        ~Graphics();

        // no copy / no move policy
        Graphics(const Graphics& other)=delete;
        Graphics(Graphics&& other)=delete;
        Graphics& operator=(const Graphics& other)=delete;
        Graphics& operator=(Graphics&& other)=delete;

        // behavior
        bool Init();
        void Update();
        
        // getters / setters
        SDL_Window* getWindow();
        SDL_Renderer* getRenderer();

    private:
        // window size
        static constexpr float _aspectRatio = 4 / 3;
        static constexpr int _width = 700.0f;
        static constexpr int _height = _width * _aspectRatio;

        // frame rate
        static constexpr int _frameRate = 60;
        static constexpr int _frameDuration = 1000 / _frameRate;

        // SDL window and renderer
        const std::string _title { "Pacman" };
        std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> _renderer { NULL, SDL_DestroyRenderer };
        std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> _window { NULL,  SDL_DestroyWindow };

};

#endif // GRAPHICS_H