#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL2/SDL.h>
#include <string>

#include "Entity.h"

class Graphics
{

    public:
        // constructor / destructor
        Graphics();
        Graphics(std::string title, float width, float height, float aspectRatio, int frameRate);
        ~Graphics();

        // no copy/move policy
        Graphics(Graphics&& other)=delete;
        Graphics(const Graphics& other)=delete;
        Graphics& operator=(Graphics&& other)=delete;
        Graphics& operator=(const Graphics& other)=delete;

        // behavior
        bool Init();
        void Clear();
        void Render(const Entity& entity);
        std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> LoadTexture(std::string filename);
        void Present();

        // setters / getters
        int GetFrameRate();
        int GetFrameDuration();

    private:
        // window size
        float _aspectRatio, _width, _height;
        // frame rate
        int _frameRate, _frameDuration;
        // pause flag
        bool _isPaused = false;
        // SDL window and renderer
        std::string _title;
        std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> _window { NULL,  SDL_DestroyWindow };
        std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> _renderer { NULL, SDL_DestroyRenderer };

};

#endif // GRAPHICS_H