#include "NPCSystem.h"
#include <glm/gtx/norm.hpp>

namespace Game::NPC
{
    float NPCSystem::DistanceSq(const glm::vec3& a, const glm::vec3& b) noexcept
    {
        return glm::distance2(a, b);
    }

    NPCState NPCSystem::EvaluateState(const NPCComponent& npc, const glm::vec3& playerPos) noexcept
    {
        const float distSq   = DistanceSq(npc.position, playerPos);
        const float aggroSq  = npc.tuning->aggroRange  * npc.tuning->aggroRange;
        const float attackSq = npc.tuning->attackRange * npc.tuning->attackRange;

        if (distSq <= attackSq)
            return NPCState::Attack;

        if (distSq <= aggroSq)
            return NPCState::Chase;

        return NPCState::Wander;
    }

    void NPCSystem::Update(std::span<NPCComponent> npcs, const glm::vec3& playerPos, float dt) noexcept
    {
        for (auto& npc : npcs)
        {
            npc.state = EvaluateState(npc, playerPos);

            switch (npc.state)
            {
                case NPCState::Idle:   UpdateIdle(npc, playerPos, dt); break;
                case NPCState::Wander: UpdateWander(npc, playerPos, dt); break;
                case NPCState::Chase:  UpdateChase(npc, playerPos, dt); break;
                case NPCState::Attack: UpdateAttack(npc, playerPos, dt); break;
            }
        }
    }

    void NPCSystem::UpdateIdle(NPCComponent& npc, const glm::vec3&, float dt) noexcept
    {
        npc.velocity = glm::vec3(0.0f);
        npc.stateTimer += dt;
    }

    void NPCSystem::UpdateWander(NPCComponent& npc, const glm::vec3&, float dt) noexcept
    {
        npc.position += npc.velocity * dt;
        npc.stateTimer += dt;
    }

    void NPCSystem::UpdateChase(NPCComponent& npc, const glm::vec3& playerPos, float dt) noexcept
    {
        glm::vec3 dir    = glm::normalize(playerPos - npc.position);
        npc.velocity     = dir * npc.tuning->moveSpeed;
        npc.position    += npc.velocity * dt;
        npc.stateTimer  += dt;
    }

    void NPCSystem::UpdateAttack(NPCComponent& npc, const glm::vec3&, float dt) noexcept
    {
        npc.stateTimer += dt;
    }
}