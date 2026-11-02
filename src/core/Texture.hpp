#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <stdexcept>

// Automatically destroys the texture when it goes out of scope (RAII)
class Texture {
    SDL_Texture* m_handle;

public:
    Texture(SDL_Renderer* renderer, const char* path)
        : m_handle(IMG_LoadTexture(renderer, path))
    {
        // SDL3_image uses SDL_GetError(), not IMG_GetError()
        if (!m_handle) {
            throw std::runtime_error(SDL_GetError());
        }
    }

    ~Texture() {
        if (m_handle) {
            SDL_DestroyTexture(m_handle);
        }
    }

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    Texture(Texture&& other) noexcept : m_handle(other.m_handle) {
        other.m_handle = nullptr;
    }

    Texture& operator=(Texture&& other) noexcept {
        if (this != &other) {
            if (m_handle) SDL_DestroyTexture(m_handle);
            m_handle = other.m_handle;
            other.m_handle = nullptr;
        }
        return *this;
    }

    SDL_Texture* get() const { return m_handle; }
};
