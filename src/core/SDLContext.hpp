#pragma once

#include <SDL3/SDL.h>
#include <iostream>
#include <stdexcept>

class SDLContext
{
    public:
        SDLContext() 
        {
        if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) 
        {
            throw std::runtime_error(SDL_GetError());
        }
        }

        ~SDLContext() 
        {
            SDL_Quit();
        }

        // No copy, no move for singleton-like contexts
        SDLContext(const SDLContext&) = delete;
        SDLContext& operator=(const SDLContext&) = delete;
};