#include "Game.hpp"
#include <SDL3/SDL_events.h>
#include <memory>      // Required for std::make_unique
#include <iostream>
#include <algorithm>   // Required for std::clamp

Game::Game()
    : m_sdl(SDL_INIT_VIDEO)
    , m_window("My Game", 800, 600)
    , m_renderer(m_window)
{
    registerHandlers();

    try {
        m_texture = std::make_unique<Texture>(
            m_renderer.get(), "assets/images/player.png"
        );
    } catch (const std::exception& e) {
        std::cerr << "Warning: could not load texture: " << e.what() << '\n';
        // m_texture stays nullptr — game continues with a white rectangle
    }
}

// Destructor defined ONCE here; the = default in Game.hpp is removed
Game::~Game() = default;

void Game::registerHandlers()
{
    m_eventLoop.on(SDL_EVENT_QUIT, [this](const SDL_Event&) {
        m_eventLoop.request_quit();
    });

    m_eventLoop.on(SDL_EVENT_WINDOW_CLOSE_REQUESTED, [this](const SDL_Event& e) {
        m_eventLoop.request_quit();
    });

    m_eventLoop.on(SDL_EVENT_KEY_DOWN, [this](const SDL_Event& e) {
        if (e.key.scancode == SDL_SCANCODE_ESCAPE)
            m_eventLoop.request_quit();
    });
}

void Game::run()
{
    Uint64 lastTicks = SDL_GetTicks();

    while (!m_eventLoop.quit_requested()) {
        Uint64 currentTicks = SDL_GetTicks();
        float deltaTime = (currentTicks - lastTicks) / 1000.0f;
        lastTicks = currentTicks;

        // Cap delta time to prevent large jumps when the window is moved
        if (deltaTime > 0.1f) deltaTime = 0.1f;

        m_eventLoop.poll();
        update(deltaTime);
        render();
    }
}

void Game::update(float deltaTime)
{
    const bool* keys = SDL_GetKeyboardState(nullptr);
    float speed = 300.0f; // pixels per second

    if (keys[SDL_SCANCODE_LEFT])  m_playerX -= speed * deltaTime;
    if (keys[SDL_SCANCODE_RIGHT]) m_playerX += speed * deltaTime;
    if (keys[SDL_SCANCODE_UP])    m_playerY -= speed * deltaTime;
    if (keys[SDL_SCANCODE_DOWN])  m_playerY += speed * deltaTime;

    // Boundary check to keep the player inside the 800x600 window
    m_playerX = std::clamp(m_playerX, 0.0f, static_cast<float>(800) - m_texW);
    m_playerY = std::clamp(m_playerY, 0.0f, static_cast<float>(600) - m_texH);
}

void Game::render()
{
    m_renderer.clear(SDL_Color{30, 30, 30, 255});

    if (m_texture) {
        SDL_FRect dest = { m_playerX, m_playerY, m_texW, m_texH };
        m_renderer.draw_texture(m_texture->get(), &dest);
    } else {
        // Fallback: white rectangle when texture is missing
        SDL_FRect rect = { m_playerX, m_playerY, m_texW, m_texH };
        m_renderer.fill_rect(rect, SDL_Color{255, 255, 255, 255});
    }

    m_renderer.present();
}
