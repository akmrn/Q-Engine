#include "Window.hpp"

#include <utility>

namespace app
{

    Window::Window(std::string_view title, int width, int height, SDL_WindowFlags flags)
    {
        // SDL3 takes a C string; string_view is not guaranteed NUL-terminated.
        const std::string owned{title};
        window_ = SDL_CreateWindow(owned.c_str(), width, height, flags);
        if (!window_)
        {
            throw std::runtime_error(
                std::format("SDL_CreateWindow failed: {}", SDL_GetError()));
        }
    }

    Window::~Window()
    {
        SDL_DestroyWindow(window_);
    }

    Window::Window(Window&& other) noexcept
        : window_ {std::exchange(other.window_, nullptr)}
    {}

    Window& Window::operator = (Window&& other) noexcept
    {
        if (this != &other)
        {
            SDL_DestroyWindow(window_);
            window_ = std::exchange(other.window_, nullptr);
        }
        return *this;
    }

    SDL_Window* Window::release() noexcept
    {
        return std::exchange(window_, nullptr);
    }

    void Window::reset(SDL_Window* window) noexcept
    {
        if (window_ != window)
        {
            SDL_DestroyWindow(window_);
            window_ = window;
        }
    }

    std::pair<int, int> Window::size() const noexcept
    {
        int w = 0, h = 0;
        if (window_)
        {
          SDL_GetWindowSize(window_, &w, &h);
        }
        return {w, h};
    }

    std::pair<int, int> Window::pixel_size() const noexcept
    {
        int w = 0, h = 0;
        SDL_GetWindowSizeInPixels(window_, &w, &h);
        return {w, h};
    }

    void Window::set_title(std::string_view title)
    {
        if (!window_)
        {
          return; // یا اگر ترجیح می‌دهید خطا اعلام شود، throw std::logic_error
        }

        const std::string owned{title};
        if (!SDL_SetWindowTitle(window_, owned.c_str()))
        {
            throw std::runtime_error(
                std::format("SDL_SetWindowTitle failed: {}", SDL_GetError()));
        }
    }

} // namespace app
