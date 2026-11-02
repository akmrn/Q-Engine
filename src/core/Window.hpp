#pragma once

#include <SDL3/SDL.h>
#include <iostream>
#include <stdexcept>

class Window {
    SDL_Window* m_handle;
public:
    Window(const char* title, int w, int h)
        : m_handle(SDL_CreateWindow(title, w, h, 0))
    {
        if (!m_handle) throw std::runtime_error(SDL_GetError());
    }
    ~Window() { SDL_DestroyWindow(m_handle); }

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    SDL_Window* get() const { return m_handle; }
};