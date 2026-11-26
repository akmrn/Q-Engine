#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <memory>                    // Required for std::unique_ptr

#include "platform/SDLContext.hpp"
#include "platform/Window.hpp"
#include "renderer/Renderer.hpp"
#include "renderer/Texture.hpp"
#include "events/EventLoop.hpp"

// The main game class — owns the window, renderer, and game state
class Game {
    Qeng::SDLContext m_sdl;
    Qeng::Window     m_window;
    Qeng::Renderer   m_renderer;

    std::unique_ptr<Texture> m_texture;

    Qeng::EventLoop   m_eventLoop;

    float m_playerX = 350.0f;
    float m_playerY = 250.0f;
    float m_texW    = 100.0f;
    float m_texH    = 100.0f;

public:
    Game();
    ~Game();   // Declared here, defined once in Game.cpp

    void run();

private:
    void registerHandlers();
    void update(float deltaTime);
    void render();
};
