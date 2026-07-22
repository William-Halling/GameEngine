#pragma once
#include "Input.h"
#include "InputSystem.h"
#include "Window.h"
#include "Player.h"
#include "NPCSystem.h"
#include "CommandBuffer.hpp"

#include <vector>
#include <chrono>
#include <memory>


class Engine
{
    public:
        Engine();
        ~Engine() = default;

        Engine(const Engine&) = delete;
        Engine& operator=(const Engine&) = delete;

        void Run();

    private:
        void Update(float dt);
        void Render();

        Core::Window              m_Window;
        Core::InputSystem         m_InputSystem;
        InputBuffers              m_InputBuffers;
        Gameplay::CommandBuffer   m_Commands;

        //PlayerData                m_Player;
        CameraData                m_Camera;

        std::vector<Game::NPC::NPCComponent> m_NPCs;
        Game::NPC::NPCBlackboard  m_NPCBlackboard;   // shared tuning

        // Timing
        std::chrono::steady_clock::time_point m_LastTime;
        bool m_Running = true;
};