#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>

#include "Input.h"
#include "CommandBuffer.hpp"


struct alignas(16) CameraData
{
    glm::vec3 position{ 0.0f, 1.6f, 5.0f };
    float _pad0 = 0.0f;

    glm::vec3 front{ 0.0f, 0.0f, -1.0f };
    float yaw = -90.0f;

    glm::vec3 up{ 0.0f, 1.0f, 0.0f };
    float pitch = 0.0f;

    glm::vec3 right{ 1.0f, 0.0f, 0.0f };
    float     zoom = 45.0f;

    glm::vec3 worldUp{ 0.0f, 1.0f, 0.0f };
    float     movementSpeed = 6.0f;

    float     mouseSensitivity = 0.08f;
    float     _pad1[3] = {};
};

namespace Camera
{
    inline void UpdateBasis(CameraData& c) noexcept
    {
        const float yawRad = glm::radians(c.yaw);
        const float pitchRad = glm::radians(c.pitch);

        glm::vec3 front;
        front.x = cos(yawRad) * cos(pitchRad);
        front.y = sin(pitchRad);
        front.z = sin(yawRad) * cos(pitchRad);
        c.front = glm::normalize(front);

        c.right = glm::normalize(glm::cross(c.front, c.worldUp));
        c.up = glm::normalize(glm::cross(c.right, c.front));
    }

    [[nodiscard]] inline glm::mat4 View(const CameraData& c) noexcept
    {
        return glm::lookAt(c.position, c.position + c.front, c.up);
    }

    [[nodiscard]] inline glm::mat4 Projection(const CameraData& c, float aspect, float nearPlane = 0.1f, float farPlane = 2000.0f) noexcept
    {
        return glm::perspective(glm::radians(c.zoom), aspect, nearPlane, farPlane);
    }

    inline void AddYawPitch(CameraData& c, float deltaYaw, float deltaPitch) noexcept
    {
        c.yaw += deltaYaw * c.mouseSensitivity;
        c.pitch += deltaPitch * c.mouseSensitivity;
        c.pitch = glm::clamp(c.pitch, -89.0f, 89.0f);
        UpdateBasis(c);
    }

    inline void Move(CameraData& c, const glm::vec3& localDir, float dt) noexcept
    {
        c.position += localDir * (c.movementSpeed * dt);
    }
}

class CameraSystem
{
public:
    static inline void Update(CameraData& cam, const InputBuffers& input, float dt) noexcept
    {
        glm::vec3 moveDir{ 0.0f };

        glm::vec3 moveDir{ 0.0f };

        for (const auto& m : commands.moves)
            moveDir += m.dir;

        if (glm::length2(moveDir) > 0.0f)
            moveDir = glm::normalize(moveDir);

        Camera::Move(cam, moveDir, dt);

        for (const auto& l : commands.looks)
            Camera::AddYawPitch(cam, l.yawDelta, l.pitchDelta);
    }
};
