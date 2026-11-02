#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <stdexcept>

// Automatically destroys the texture when it goes out of scope
class Texture {
    SDL_Texture* m_handle;

public:
    // Load an image file and create a GPU texture from it
    Texture(SDL_Renderer* renderer, const char* path)
        : m_handle(IMG_LoadTexture(renderer, path))
    {
        if (!m_handle) {
            throw std::runtime_error(IMG_GetError());
        }
    }

    ~Texture() {
        if (m_handle) {
            SDL_DestroyTexture(m_handle);
        }
    }

    // Prevent copying — a texture is a unique GPU resource
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    // Allow moving — transfer ownership of the GPU resource
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
