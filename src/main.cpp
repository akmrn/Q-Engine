<<<<<<< HEAD
#include "Game.hpp"
#include <iostream>

int main() {
    try {
        Game game;
        game.run();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << '\n';
        return 1;
    }
    return 0;
} // main
=======
#include "core/EngineCore.hpp"
#include "core/ImGuiLayer.hpp"
#include <SDL3/SDL.h>
#include <iostream>

int main(int argc, char* argv[])
{
    try
    {
        // Step 1: Initialize Engine (Window + Renderer)
        EngineCore engine("2D Engine with SDL3 & ImGui", 1280, 720);

        // Step 2: Initialize UI Debug Layer via RAII
        ImGuiLayer uiLayer(engine.getWindow(), engine.getRenderer());

        bool isRunning = true;
        SDL_Event event;

        // High-resolution timing variables for Delta Time calculation
        Uint64 lastCounter = SDL_GetPerformanceCounter();
        const double frequency = static_cast<double>(SDL_GetPerformanceFrequency());

        // Game state variables for debugging via ImGui
        float clearColor[3] = {0.1f, 0.15f, 0.25f}; // Background color (RGB normalized)
        float playerSpeed = 200.0f;                  // Pixels per second
        float playerX = 100.0f;                      // Player X position

        // Main Game Loop
        while (isRunning)
        {
            // Calculate Delta Time in seconds
            Uint64 currentCounter = SDL_GetPerformanceCounter();
            float deltaTime = static_cast<float>((currentCounter - lastCounter) / frequency);
            lastCounter = currentCounter;

            // Poll input events from the operating system queue
            while (SDL_PollEvent(&event))
            {
                // Pass event to ImGui first
                uiLayer.processEvent(event);

                if (event.type == SDL_EVENT_QUIT) 
                {
                    isRunning = false;
                }
            } // eventLoop end

            // ---- Update / Physics Phase ----
            // Move player smoothly using Delta Time
            playerX += playerSpeed * deltaTime;
            if (playerX > 1200.0f) {
                playerX = 50.0f; // Reset position to loop across screen
            }

            // ---- UI Construction Phase ----
            uiLayer.beginFrame();

            // Create a debug control panel
            ImGui::Begin("Engine Control & Diagnostics");
            ImGui::Text("Application Performance: %.1f FPS", 1.0f / deltaTime);
            ImGui::Text("Delta Time: %.4f seconds", deltaTime);
            ImGui::Separator();
            ImGui::ColorEdit3("Clear Color", clearColor);
            ImGui::SliderFloat("Player Speed", &playerSpeed, 0.0f, 1000.0f);
            ImGui::Text("Player X: %.2f", playerX);
            ImGui::End();

            // ---- Rendering Phase ----
            // Clear screen with current color settings
            SDL_SetRenderDrawColor(engine.getRenderer(),
                static_cast<Uint8>(clearColor[0] * 255.0f),
                static_cast<Uint8>(clearColor[1] * 255.0f),
                static_cast<Uint8>(clearColor[2] * 255.0f),
                255);
            SDL_RenderClear(engine.getRenderer());

            // Draw a simple debug quad representing the player
            SDL_FRect playerRect = { playerX, 300.0f, 64.0f, 64.0f };
            SDL_SetRenderDrawColor(engine.getRenderer(), 220, 38, 38, 255); // Red color
            SDL_RenderFillRect(engine.getRenderer(), &playerRect);

            // Render UI over the game world
            uiLayer.endFrame();

            // Present the back buffer to the screen (Double Buffering)
            SDL_RenderPresent(engine.getRenderer());
        } // gameLoop end
    } catch (const std::exception& e) 
    {
        std::cerr << "FATAL ERROR: " << e.what() << std::endl;
        return -1;
    }

    // EngineCore goes out of scope here. The destructor automatically cleans up everything.
    return 0;
} // main
>>>>>>> 42ebc241fb68ec564903880a181a6e7d647a6f36
