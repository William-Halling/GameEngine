#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "NPCSystem.h"

namespace Game::NPC
{
	struct NPC_SOA
	{
		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> velocities;
		std::vector<float>	   health;
		std::vector<uint8_t>   state;
		std::vector<float>     stateTimer;

		NPCBlackboard* tuning = nullptr;

		void resize(size_t count)
		{
			positions.resize(count);
			velocities.resize(count);
			health.resize(count);
			state.resize(count);
			stateTimer.resize(count);
		}
	};
}