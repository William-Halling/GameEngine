#include "InputSystem.h"

namespace Core
{
	void InputSystem::Update(const Window& window, InputBuffers& buffers, Gameplay::CommandBuffer& commands) noexcept
	{
		(void)window;

		commands.clear();

		if(buffers.keys[GLFW_KEY_W]) commands.moves.push_back({ glm::vec3(0.0f, 0.0f, -1.0f) });

		if (buffers.keys[GLFW_KEY_S]) commands.moves.push_back({ glm::vec3(0.0f, 0.0f, 1.0f) });

		if (buffers.keys[GLFW_KEY_A]) commands.moves.push_back({ glm::vec3(-1.0f, 0.0f, 0.0f) });

		if (buffers.keys[GLFW_KEY_D]) commands.moves.push_back({ glm::vec3(1.0f, 0.0f, 0.0f) });


		if (buffers.mouseDeltaX != 0.0 || buffers.mouseDeltaY != 0.0)
		{
			commands.looks.push_back({
				static_cast<float>(buffers.mouseDeltaX),
				static_cast<float>(buffers.mouseDeltaY)
			});
		}
	}
}