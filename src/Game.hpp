#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <memory>                    // Required for std::unique_ptr
#include "core/SDLContext.hpp"
#include "core/Window.hpp"
#include "core/Renderer.hpp"
#include "core/Texture.hpp"

// The main game class — owns the window, renderer, and game state
class Game {
    app::SDLContext m_sdl;
    app::Window     m_window;
    app::Renderer   m_renderer;

    std::unique_ptr<Texture> m_texture;

    float m_playerX = 350.0f;
    float m_playerY = 250.0f;
    float m_texW    = 100.0f;
    float m_texH    = 100.0f;

    bool m_running = true;

public:
    Game();
    ~Game();   // Declared here, defined once in Game.cpp

    void run();

private:
    void handleEvents();
    void update(float deltaTime);
    void render();
};
