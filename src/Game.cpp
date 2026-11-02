#include "Game.hpp"
#include <iostream>

Game::Game()
    : m_sdl()
    , m_window("My Game", 800, 600)
    , m_renderer(m_window.get())
{
    // Initialize SDL3_image with PNG and JPEG support
    if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG)) {
        throw std::runtime_error(IMG_GetError());
    }

    // Load a texture — put any image file in your assets/ folder
    // For now we create it later in a try block; if the file is missing
    // the game still runs, just without the texture
    try {
        m_texture = std::make_unique<Texture>(
            m_renderer.get(), "assets/images/player.png"
        );
    } catch (const std::exception& e) {
        std::cerr << "Warning: could not load texture: " << e.what() << '\n';
        // m_texture stays nullptr — game continues without it
    }
}

void Game::run() {
    Uint64 lastTicks = SDL_GetTicks();

    // Main game loop
    while (m_running) {
        // Calculate delta time in seconds
        Uint64 currentTicks = SDL_GetTicks();
        float deltaTime = (currentTicks - lastTicks) / 1000.0f;
        lastTicks = currentTicks;

        // Cap delta time to prevent huge jumps (e.g. when window is dragged)
        if (deltaTime > 0.1f) deltaTime = 0.1f;

        handleEvents();
        update(deltaTime);
        render();
    }
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            m_running = false;
        }
        if (event.type == SDL_EVENT_KEY_DOWN &&
            event.key.scancode == SDL_SCANCODE_ESCAPE) {
            m_running = false;
        }
    }
}

void Game::update(float deltaTime) {
    // Read keyboard state for smooth, continuous movement
    // SDL_GetKeyboardState returns a snapshot of all keys
    const bool* keys = SDL_GetKeyboardState(nullptr);

    float speed = 300.0f;  // pixels per second

    // Move player based on arrow keys, scaled by deltaTime
    if (keys[SDL_SCANCODE_LEFT])  m_playerX -= speed * deltaTime;
    if (keys[SDL_SCANCODE_RIGHT]) m_playerX += speed * deltaTime;
    if (keys[SDL_SCANCODE_UP])    m_playerY -= speed * deltaTime;
    if (keys[SDL_SCANCODE_DOWN])  m_playerY += speed * deltaTime;
}

void Game::render() {
    // Clear screen with dark gray
    SDL_SetRenderDrawColor(m_renderer.get(), 30, 30, 30, 255);
    SDL_RenderClear(m_renderer.get());

    // Draw the texture if it loaded successfully
    if (m_texture) {
        SDL_FRect dest = { m_playerX, m_playerY, m_texW, m_texH };
        SDL_RenderTexture(m_renderer.get(), m_texture->get(), nullptr, &dest);
    } else {
        // Fallback: draw a white rectangle if no texture
        SDL_SetRenderDrawColor(m_renderer.get(), 255, 255, 255, 255);
        SDL_FRect rect = { m_playerX, m_playerY, m_texW, m_texH };
        SDL_RenderFillRect(m_renderer.get(), &rect);
    }

    SDL_RenderPresent(m_renderer.get());
}

Game::~Game() {
    // Smart pointer cleans up texture automatically
    // Renderer, Window, SDLContext clean up via their own RAII destructors
    IMG_Quit();
}
