#include "Engine.h"
#include <glad/glad.h>

Engine::Engine()
	: m_Window(Core::Window::Desc{})
{
	m_LastTime = std::chrono::steady_clock::now();

	m_NPCBlackboard = Game::NPC::NPCBlackboard{};
	m_NPCs.resize(64);

	for (auto& npc : m_NPCs)
		npc.tuning = &m_NPCBlackboard;
}

void Engine::Run()
{
    using clock = std::chrono::steady_clock;

    constexpr float fixedDt = 1.0f / 60.0f;
    float accumulator = 0.0f;

    auto prev = clock::now();

    while (!m_Window.ShouldClose() && m_Running)
    {
        auto now = clock::now();
        float frameDt = std::chrono::duration<float>(now - prev).count();
        prev = now;

        accumulator += frameDt;

        m_InputBuffers.BeginFrame();
        m_Window.PollEvents();

        m_InputSystem.Update(m_Window, m_InputBuffers, m_Commands);

        while (accumulator >= fixedDt)
        {
            Update(fixedDt);
            accumulator -= fixedDt;
        }

        Render();
        m_Window.SwapBuffers();
    }
}


void Engine::Update(float dt)
{
    CameraSystem::Update(m_Camera, m_Commands, dt);

    Game::NPC::NPCSystem::Update(std::span<Game::NPC::NPCComponent>(m_NPCs),m_Camera.position, dt);
}