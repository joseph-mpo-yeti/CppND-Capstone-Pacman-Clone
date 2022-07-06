#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL2/SDL.h>

#include "Physics.h"

class Graphics
{

    public:
        // constructor / destructor
        Graphics();
        Graphics(std::string title, float width, float height, float aspectRatio, int frameRate);
        ~Graphics();

        // no copy policy
        Graphics(const Graphics& other)=delete;
        Graphics& operator=(const Graphics& other)=delete;
        
        // move constructor / move assignment operator
        Graphics(Graphics&& other);
        Graphics& operator=(Graphics&& other);

        // behavior
        bool Init();
        void Update();
        void Pause();
        void Resume();
    
    private:
        // getters / setters
        SDL_Window* getWindow();
        SDL_Renderer* getRenderer();

        // TODO: move into controller/player/entity
        void InitPlayer();

    private:
        // window size
        float _aspectRatio, _width, _height;

        // frame rate
        int _frameRate, _frameDuration;

        // pause flag
        bool _isPaused = false;

        // Player
        // TODO: Move into player (to be updated later)
        SDL_Rect _rect;
        Velocity _velocity;
        int _speed;

        // SDL window and renderer
        std::string _title;
        std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> _window { NULL,  SDL_DestroyWindow };
        std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> _renderer { NULL, SDL_DestroyRenderer };

};

#endif // GRAPHICS_H