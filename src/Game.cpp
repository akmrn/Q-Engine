#include "Game.hpp"
#include <memory>      // Required for std::make_unique
#include <iostream>

Game::Game()
    : m_sdl(SDL_INIT_VIDEO)
    , m_window("My Game", 800, 600)
    , m_renderer(m_window)
{
    // SDL3_image does NOT require IMG_Init() — it initializes automatically.
    // Just load textures directly.
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
Game::~Game() {
    // std::unique_ptr cleans up the texture automatically (RAII)
    // Renderer, Window, SDLContext each clean up in their own destructors
}

void Game::run() {
    Uint64 lastTicks = SDL_GetTicks();

    while (m_running) {
        Uint64 currentTicks = SDL_GetTicks();
        float deltaTime = (currentTicks - lastTicks) / 1000.0f;
        lastTicks = currentTicks;

        // Cap delta time to prevent large jumps when the window is moved
        if (deltaTime > 0.1f) deltaTime = 0.1f;

        handleEvents();
        update(deltaTime);
        render();
    }
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT)
            m_running = false;

        if (event.type == SDL_EVENT_KEY_DOWN &&
            event.key.scancode == SDL_SCANCODE_ESCAPE)
            m_running = false;
    }
}

void Game::update(float deltaTime) {
    const bool* keys = SDL_GetKeyboardState(nullptr);
    float speed = 300.0f; // pixels per second

    if (keys[SDL_SCANCODE_LEFT])  m_playerX -= speed * deltaTime;
    if (keys[SDL_SCANCODE_RIGHT]) m_playerX += speed * deltaTime;
    if (keys[SDL_SCANCODE_UP])    m_playerY -= speed * deltaTime;
    if (keys[SDL_SCANCODE_DOWN])  m_playerY += speed * deltaTime;
}

void Game::render() {
    SDL_SetRenderDrawColor(m_renderer.get(), 30, 30, 30, 255);
    SDL_RenderClear(m_renderer.get());

    if (m_texture) {
        SDL_FRect dest = { m_playerX, m_playerY, m_texW, m_texH };
        SDL_RenderTexture(m_renderer.get(), m_texture->get(), nullptr, &dest);
    } else {
        // Fallback: white rectangle when texture is missing
        SDL_SetRenderDrawColor(m_renderer.get(), 255, 255, 255, 255);
        SDL_FRect rect = { m_playerX, m_playerY, m_texW, m_texH };
        SDL_RenderFillRect(m_renderer.get(), &rect);
    }

    SDL_RenderPresent(m_renderer.get());
}
