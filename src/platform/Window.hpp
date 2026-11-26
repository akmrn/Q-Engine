#pragma once

#include <SDL3/SDL.h>

#include <format>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

namespace app
{

    class Window
    {
        public:
            Window(std::string_view title, int width, int height, SDL_WindowFlags flags = 0);
            ~Window();

            Window(const Window&)              = delete;
            Window& operator = (const Window&) = delete;

            Window(Window&&)            noexcept;
            Window& operator=(Window&&) noexcept;

            [[nodiscard]] SDL_Window* get() const noexcept { return window_; }
            explicit operator bool() const noexcept { return window_ != nullptr; }

            /// Give up ownership without destroying (for legacy C APIs).
            [[nodiscard]] SDL_Window* release() noexcept;
            /// Destroy current window (if any) and adopt a new handle.
            void reset(SDL_Window* window = nullptr) noexcept;

            /// Logical window size in screen coordinates.
            [[nodiscard]] std::pair<int, int> size() const noexcept;
            /// Size in actual pixels (differs on HiDPI displays).
            [[nodiscard]] std::pair<int, int> pixel_size() const noexcept;

            void set_title(std::string_view title);

        private:
            SDL_Window* window_ = nullptr;
};

} // namespace app
