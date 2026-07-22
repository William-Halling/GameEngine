#pragma once
#include "Input.h"
#include "Window.h"
#include "CommandBuffer.hpp"

namespace Core
{
	class InputSystem
	{
	public:
		InputSystem() = default;

		void Update(const Window& window, InputBuffers& buffers, Gameplay::CommandBuffer& commands) noexcept;
	};
}