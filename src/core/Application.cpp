#include "Application.hpp"

#include <iostream>
#include <exception>

namespace Qeng
{
    Application::Application()
    {
        // Game construction is deferred or safely enclosed
        // to catch initialization errors (e.g. video driver, window, or renderer failure).
    }

    int Application::run()
    {
        try {
            // Construct the Game instance RAII-style upon starting the application loop.
            // SDLContext, Window, Renderer, and Texture are initialized here.
            m_game.emplace();

            // Hand off execution to the game loop
            m_game->run();
        }
        catch (const std::exception& e) {
            std::cerr << "[Critical Error] Application exception caught: " << e.what() << '\n';
            return 1;
        }
        catch (...) {
            std::cerr << "[Critical Error] Unknown exception caught during application execution.\n";
            return 2;
        }

        // Resources are automatically cleaned up in reverse order of construction
        // via RAII when m_game is destroyed or goes out of scope here.
        return 0;
    }
} // namespace Qeng
