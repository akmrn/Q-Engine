#pragma once

#include <SDL3/SDL.h>
#include <iostream>
#include <stdexcept>

#include "Window.hpp"

class Renderer
{
    SDL_Renderer* m_handle;
public:
    explicit Renderer(SDL_Window* window)
        : m_handle(SDL_CreateRenderer(window, nullptr))
    {
        if (!m_handle) throw std::runtime_error(SDL_GetError());
    }

    ~Renderer() { SDL_DestroyRenderer(m_handle); }

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    SDL_Renderer* get() const { return m_handle; }
};