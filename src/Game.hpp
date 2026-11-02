#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "core/SDLContext.hpp"
#include "core/Window.hpp"
#include "core/Renderer.hpp"
#include "core/Texture.hpp"

// The main game class — owns the window, renderer, and game state
// Encapsulates the entire game loop behind a simple run() interface
class Game {
    SDLContext m_sdl;
    Window     m_window;
    Renderer   m_renderer;

    // A texture we will load and draw
    std::unique_ptr<Texture> m_texture;

    // Player position — will move using delta time
    float m_playerX = 350.0f;
    float m_playerY = 250.0f;

    // Texture draw size
    float m_texW = 100.0f;
    float m_texH = 100.0f;

    bool m_running = true;

public:
    Game();
    ~Game() = default;

    // Start the game loop — runs until the user closes the window
    void run();

private:
    // Process all pending input events
    void handleEvents();

    // Update game logic, given the time elapsed since last frame
    void update(float deltaTime);

    // Draw everything to the screen
    void render();
};
