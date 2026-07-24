#include "Engine.h"
#include <glad/glad.h>


Engine::Engine()
	: m_Window(Core::Window::Desc{}), m_RenderSystem(m_Window)
{
	m_LastTime = std::chrono::steady_clock::now();

	m_NPCBlackboard = Game::NPC::NPCBlackboard{};
	
    m_NPCs.resize(64);

    //m_Lua.LoadScript("scripts/npc.lua");
    //RegisterNPCBindings(m_Lua.State());

	for (auto& npc : m_NPCs)
		npc.tuning = &m_NPCBlackboard;
}


void Engine::Render()
{
    m_RenderSystem.BeginFrame();



    m_RenderSystem.EndFrame();
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
        // Camera consumes commands
    CameraSystem::Update(m_Camera, m_Commands, dt);

        // NPCs updated in parallel
    auto span = std::span<Game::NPC::NPCComponent>(m_NPCs);
    std::uint32_t count = static_cast<std::uint32_t>(span.size());

    m_JobSystem.ParallelFor(count, [&](std::uint32_t i)
    {
        Game::NPC::NPCSystem::UpdateSingle(span[i], m_Camera.position, dt);
    });
}