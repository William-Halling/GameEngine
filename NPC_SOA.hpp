#pragma once

#include <vector>
#include <cstddef>
#include <glm/glm.hpp>

namespace Game::NPC
{
    struct NPCBlackboard; // forward

    struct NPC_SOA
    {
        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> velocities;
        std::vector<float>     health;
        std::vector<uint8_t>   state;
        std::vector<float>     stateTimer;

        NPCBlackboard* tuning = nullptr;

        void resize(std::size_t count)
        {
            positions.resize(count);
            velocities.resize(count);
            health.resize(count);
            state.resize(count);
            stateTimer.resize(count);
        }

        void reserve(std::size_t count)
        {
            positions.reserve(count);
            velocities.reserve(count);
            health.reserve(count);
            state.reserve(count);
            stateTimer.reserve(count);
        }

        std::size_t size() const noexcept { return positions.size(); }
    };
}