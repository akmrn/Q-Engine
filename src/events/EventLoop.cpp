#include "EventLoop.hpp"

#include <utility>

namespace app
{

    void EventLoop::on(SDL_EventType type, Handler handler)
    {
        handlers_[static_cast<Uint32>(type)].push_back(std::move(handler));
    }

    void EventLoop::on_any(Handler handler)
    {
        any_handlers_.push_back(std::move(handler));
    }

    void EventLoop::poll()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                quit_ = true;
            }

            for (const auto& h : any_handlers_)
            {
                h(event);
            }

            if (auto it = handlers_.find(event.type); it != handlers_.end())
            {
                for (const auto& h : it->second)
                {
                    h(event);
                }
            }
        }
    }

} // namespace app
