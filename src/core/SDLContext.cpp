#include "SDLContext.hpp"

#include <stdexcept>
#include <format>

namespace app
{

    SDLContext::SDLContext(SDL_InitFlags flags) : m_flags(flags)
    {
        //SDL3 : SDL_Init returns bool - false means failure.
        if (!SDL_Init(flags))
        {
            throw std::runtime_error(
                std::format("SDL_Init failed: {}", SDL_GetError()));
        }
    }

    SDLContext::~SDLContext()
    {
        SDL_Quit();
    }

    Uint32 SDLContext::initialized_subsystems() const noexcept
    {
        return SDL_WasInit(m_flags);
    }

    bool SDLContext::is_initialized(SDL_InitFlags flags) const noexcept
    {
        // SDL_WasInit returns a bitmask; (result & flags) == flags
        // means every requested subsystem is live.
        return (SDL_WasInit(flags) & flags) == flags;
    }

} // namespace app
