#pragma once

#include <SDL3/SDL.h>

namespace app
{
/// RAII wrapper around SDL_Init / SDL_Quit.
///
/// Owns the global SDL lifecycle.  Construct one instance at the start
/// of main(); SDL shuts down automatically when the instance is destroyed.
    class SDLContext
    {
        public:
        /// Initialize SDL with the given subsystem flags (e.g. SDL_INIT_VIDEO).
        /// Throws std::runtime_error on failure.
            explicit SDLContext(SDL_InitFlags flags);  // calls SDL_Init; throws on failure

            ~SDLContext();                             // calls SDL_Quit

            // Non-copyable: SDL init/quit is a single global resource.
            SDLContext(const SDLContext&)              = delete;
            SDLContext& operator = (const SDLContext&) = delete;

            // Non-movable: the C library holds global state; moving is meaningless.
            SDLContext(SDLContext &&)             noexcept = delete;
            SDLContext& operator = (SDLContext&&) noexcept = delete;

            /// Bitmask of currently initialized subsystems matching our flags.
            [[nodiscard]] Uint32 initialized_subsystems() const noexcept;

            /// True if every bit in `flags` is currently initialized.
            [[nodiscard]] bool is_initialized(SDL_InitFlags flags) const noexcept;

        private:
            SDL_InitFlags m_flags;
    };

} // namespace app
