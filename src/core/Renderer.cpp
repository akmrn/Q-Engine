#include "Renderer.hpp"

#include <format>
#include <stdexcept>
#include <utility>

namespace app
{

    Renderer::Renderer(const Window& window, const char* driver)
    {
        renderer_ = SDL_CreateRenderer(window.get(), driver);
        if (!renderer_)
        {
            throw std::runtime_error(
                std::format("SDL_CreateRenderer failed: {}", SDL_GetError()));
        }
    }

    Renderer::~Renderer()
    {
        SDL_DestroyRenderer(renderer_);
    }

    Renderer::Renderer(Renderer&& other) noexcept
        : renderer_{std::exchange(other.renderer_, nullptr)}
    {}

    Renderer& Renderer::operator=(Renderer&& other) noexcept
    {
        if (this != &other)
        {
            SDL_DestroyRenderer(renderer_);
            renderer_ = std::exchange(other.renderer_, nullptr);
        }
        return *this;
    }

    SDL_Renderer* Renderer::release() noexcept
    {
        return std::exchange(renderer_, nullptr);
    }

    void Renderer::reset(SDL_Renderer* renderer) noexcept
    {
        if (renderer_ != renderer)
        {
            SDL_DestroyRenderer(renderer_);
            renderer_ = renderer;
        }
    }

    void Renderer::clear(SDL_Color color)
    {
        if (!SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, color.a))
        {
            throw std::runtime_error(
            std::format("SDL_SetRenderDrawColor failed: {}", SDL_GetError()));
        }
        if (!SDL_RenderClear(renderer_))
        {
            throw std::runtime_error(
            std::format("SDL_RenderClear failed: {}", SDL_GetError()));
        }
    }

    void Renderer::present()
    {
        if (!SDL_RenderPresent(renderer_)) {
            throw std::runtime_error(
                std::format("SDL_RenderPresent failed: {}", SDL_GetError()));
        }
    }

    void Renderer::set_vsync(bool enabled)
    {
        if (!SDL_SetRenderVSync(renderer_, enabled ? 1 : 0)) {
            throw std::runtime_error(
                std::format("SDL_SetRenderVSync failed: {}", SDL_GetError()));
        }
    }

    void Renderer::set_logical_size(int width, int height)
    {
        if (!SDL_SetRenderLogicalPresentation(
                renderer_, width, height, SDL_LOGICAL_PRESENTATION_LETTERBOX)) {
            throw std::runtime_error(
                std::format("SDL_SetRenderLogicalPresentation failed: {}", SDL_GetError()));
        }
    }

    std::pair<int, int> Renderer::output_size() const noexcept
    {
        int w = 0, h = 0;
        SDL_GetCurrentRenderOutputSize(renderer_, &w, &h);
        return {w, h};
    }
} // namespace app
