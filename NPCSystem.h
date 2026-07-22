#pragma once
#include <span>
#include <cstdint>
#include <glm/glm.hpp>

namespace Game::NPC
{
    enum class NPCState : std::uint8_t
    {
        Idle,
        Wander,
        Chase,
        Attack
    };

    struct NPCBlackboard
    {
        float aggroRange    = 10.0f;
        float attackRange   = 2.0f;
        float wanderRadius  = 5.0f;
        float moveSpeed     = 3.0f;
        float idleTime      = 2.5f;
    };

    struct alignas(16) NPCComponent
    {
        glm::vec3 position{ 0.0f };
        glm::vec3 velocity{ 0.0f };
        
        float health     = 100.0f;
        NPCState state   = NPCState::Idle;
        float stateTimer = 0.0f;
        NPCBlackboard* tuning = nullptr;
    };


    class NPCSystem
    {
        public:
            static void Update(std::span<NPCComponent> npcs, const glm::vec3& playerPos, float dt) noexcept;
            static NPCState EvaluateState(const NPCComponent& npc, const glm::vec3& playerPos) noexcept;

            static void UpdateIdle(NPCComponent& npc, const glm::vec3& playerPos, float dt) noexcept;
            static void UpdateWander(NPCComponent& npc, const glm::vec3& playerPos, float dt)  noexcept;
            static void UpdateChase(NPCComponent& npc, const glm::vec3& playerPos, float dt)  noexcept;
            static void UpdateAttack(NPCComponent& npc, const glm::vec3& playerPos, float dt)  noexcept;
            static float DistanceSq(const glm::vec3& a, const glm::vec3& b) noexcept;
    };
}