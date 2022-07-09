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
        void Clear();
        void Present();
        void CloseWindow();
        void Render(const Entity& entity);
        bool PollEvent(sf::Event& event);
        std::unique_ptr<sf::Texture> LoadTexture(std::string filename);

        // setters / getters
        int GetFrameRate();
        int GetFrameDuration();
        bool WindowIsOpen();

    private:
        float _aspectRatio, _width, _height;
        int _frameRate, _frameDuration;
        bool _isPaused = false;
        std::string _title;
        std::unique_ptr<sf::RenderWindow> _window { nullptr };

};

#endif // GRAPHICS_H