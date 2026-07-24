#include "InputSystem.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <cassert>

namespace Core
{
    namespace
    {
        inline std::size_t KeyToIndex(int glfwKey) noexcept
        {
            if (glfwKey < 0) return 0;
            if (glfwKey >= 512) return 511;
            return static_cast<std::size_t>(glfwKey);
        }
    }

    void InputSystem::Update(const Window& window, InputBuffers& buffers, Gameplay::CommandBuffer& commands) noexcept
    {
        (void)window;

        commands.clear();

        const auto& keys = buffers.keys;

        glm::vec3 moveDir{0.0f};

        if (keys.test(KeyToIndex(GLFW_KEY_W))) moveDir.z -= 1.0f;
        if (keys.test(KeyToIndex(GLFW_KEY_S))) moveDir.z += 1.0f;
        if (keys.test(KeyToIndex(GLFW_KEY_A))) moveDir.x -= 1.0f;
        if (keys.test(KeyToIndex(GLFW_KEY_D))) moveDir.x += 1.0f;

        if (glm::length2(moveDir) > 0.0f)
        {
            moveDir = glm::normalize(moveDir);
            assert(commands.moves.size() < 256 && "Move command overflow");
            commands.moves.push({ moveDir });
        }

        if (buffers.mouseDeltaX != 0.0 || buffers.mouseDeltaY != 0.0)
        {
            assert(commands.looks.size() < 256 && "Look command overflow");
            commands.looks.push({
                static_cast<float>(buffers.mouseDeltaX),
                static_cast<float>(buffers.mouseDeltaY)
            });
        }
    }
}
