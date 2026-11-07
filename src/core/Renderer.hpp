#pragma once

#include <SDL3/SDL.h>

#include "Window.hpp"

#include <utility>

namespace app
{

    /// RAII owner of an SDL_Renderer.
    /// The window must outlive the renderer (Renderer keeps only a raw handle).
    class Renderer
    {
        public:
            /// `driver` may be nullptr to let SDL pick the best backend.
            explicit Renderer(const Window& window, const char* driver = nullptr);
            ~Renderer();

            Renderer(const Renderer&)            = delete;
            Renderer& operator=(const Renderer&) = delete;

            Renderer(Renderer&&) noexcept;
            Renderer& operator=(Renderer&&) noexcept;

            [[nodiscard]] SDL_Renderer* get() const noexcept { return renderer_; }
            explicit operator bool() const noexcept { return renderer_ != nullptr; }

            [[nodiscard]] SDL_Renderer* release() noexcept;
            void reset(SDL_Renderer* renderer = nullptr) noexcept;

            /// Set draw color, Clear the back buffer.
            void clear(SDL_Color color);
            /// Flip the back buffer to the screen.
            void present();

            void set_vsync(bool enabled);
            void set_logical_size(int width, int height);
            [[nodiscard]] std::pair<int, int> output_size() const noexcept;

        private:
            SDL_Renderer* renderer_ = nullptr;
    };

} // namespace app
