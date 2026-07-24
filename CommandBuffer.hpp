#pragma once
#include <vector>
#include <glm/glm.hpp>


namespace Gameplay
{
	template<typename T, size_t Capacity>
	struct RingBuffer
	{
		T data[Capacity];
		size_t head = 0;

		void clear() 
		{ 
			head = 0; 
		}

		void push(const T& cmd)
		{
			data[head % Capacity] = cmd;

			head++;
		}

		size_t size() const 
		{ 
			return head;
		}

		const T& operator[](size_t i) const
		{
			return data[i];
		}
	};

	struct MoveCommand 
	{ 
		glm::vec3 dir; 
	};

	struct LookCommand 
	{ 
		float yawDelta, pitchDelta; 
	};

	class CommandBuffer
	{
	public:

		RingBuffer<MoveCommand, 256> moves;
		RingBuffer<LookCommand, 256> looks;

		void clear() noexcept
		{
			moves.clear();
			looks.clear();
		}
	};
}