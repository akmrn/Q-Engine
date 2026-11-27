#pragma once

#include "../Game.hpp"

#include <memory>
#include <optional>

namespace Qeng
{
    /// Manages the application lifecycle, high-level process execution,
    /// and RAII-compliant ownership of the core Game instance.
    class Application
    {
        private:
            std::optional<Game> m_game;

        public:
            Application();
            ~Application() = default;

            // Non-copyable, non-movable to ensure process lifecycle uniqueness
            Application(const Application&) = delete;
            Application& operator = (const Application&) = delete;
            Application(Application&&) = delete;
            Application& operator = (Application&&) = delete;

            int run();
    };
} // namespace Qeng
