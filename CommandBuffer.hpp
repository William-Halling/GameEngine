#pragma once
#include <vector>
#include <glm/glm.hpp>


namespace Gameplay
{
	struct MoveCommand
	{
		glm::vec3 dir{};
	};

	struct LookCommand
	{
		float yawDelta = 0.0f;
		float pitchDelta = 0.0f;
	};

	class CommandBuffer
	{
	public:
		void clear() noexcept
		{
			moves.clear();
			looks.clear();
		}

		std::vector<MoveCommand> moves;
		std::vector<LookCommand> looks;
	};
}