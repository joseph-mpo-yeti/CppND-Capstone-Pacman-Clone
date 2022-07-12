#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <string>
#include <memory>
#include <SFML/Graphics.hpp>

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
        void Render(std::unique_ptr<Entity>& entity);

        // setters / getters
        int GetHeight() { return _height; };
        int GetWidth() { return _width; };
        int GetFrameRate() { return _frameRate; };
        int GetFrameDuration() { return _frameDuration; };
        sf::RenderWindow& GetWindow() { return *_window.get(); };

    private:
        float _aspectRatio, _width, _height;
        int _frameRate, _frameDuration;
        bool _isPaused = false;
        std::string _title;
        std::unique_ptr<sf::RenderWindow> _window { nullptr };

};

#endif // GRAPHICS_H