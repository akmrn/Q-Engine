#pragma once

#include <SDL3/SDL.h>

#include <functional>
#include <unordered_map>
#include <vector>

namespace Qeng
{

    /// Polls SDL events and dispatches them to registered handlers.
    /// Owns no SDL resource (SDL's queue is global, owned by SDLContext),
    /// so it must only be used while an SDLContext is alive.
    class EventLoop
    {
        public:
            using Handler = std::function<void(const SDL_Event&)>;

            EventLoop() = default;

            EventLoop(const EventLoop&)                 = delete;
            EventLoop& operator = (const EventLoop&)    = delete;
            EventLoop(EventLoop&&)                      = default;
            EventLoop& operator = (EventLoop&&)         = default;

            /// Register a handler for one event type (e.g. SDL_EVENT_KEY_DOWN).
            void on(SDL_EventType type, Handler handler);
            /// Register a handler that receives every event.
            void on_any(Handler handler);

            /// Drain the SDL event queue and dispatch. Call once per frame.
            void poll();

            /// True after SDL_EVENT_QUIT or request_quit().
            [[nodiscard]] bool quit_requested() const noexcept { return quit_; }
            void request_quit() noexcept { quit_ = true; }

            private:
                std::unordered_map<Uint32, std::vector<Handler>> handlers_;
                std::vector<Handler> any_handlers_;
                bool quit_ = false;
    };

} // namespace Qeng
