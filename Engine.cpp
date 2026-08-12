#include "Engine.h"
#include <glad/glad.h>


Engine::Engine()
{
    Core::Window::Desc desc;
    desc.width = 1280;
    desc.height = 720;
    desc.title = "Game";
    desc.vsync = true;
    desc.resizable = true;

    m_Window = std::make_unique<Core::Window>(desc);
    m_Window->Init();

        // Load GL function pointers after context creation
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        throw std::runtime_error("Failed to initialize GL loader");
    }

        // Create subsystems (RenderSystem must be after GL loader)
    m_RenderSystem = std::make_unique<Rendering::RenderSystem>();
    m_InputSystem  = std::make_unique<Core::InputSystem>();
    m_JobSystem    = std::make_unique<Core::JobSystem>(static_cast<std::uint32_t>(std::thread::hardware_concurrency()));
    m_Lua          = std::make_unique<Scripting::LuaVM>();
    m_Commands     = std::make_unique<Gameplay::CommandBuffer>();

    m_LastTime = std::chrono::steady_clock::now();

        // Example: set window user pointer if needed for callbacks
    glfwSetWindowUserPointer(m_Window->NativeHandle(), m_Window.get());
}


Engine::~Engine()
{
    Shutdown();
}


void Engine::Render()
{
    m_RenderSystem.BeginFrame();


    m_RenderSystem.EndFrame();
}


void Engine::Run()
{
    constexpr double targetDt = 1.0 / 60.0;
    while (m_Running)
    {
        auto now = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed = now - m_LastTime;
        m_LastTime = now;
        float dt = static_cast<float>(std::min(elapsed.count(), 0.25)); // clamp

        m_Window->PollEvents();
        m_InputSystem->AdvanceFrame();

        Update(dt);

        m_RenderSystem->BeginFrame();
        
        m_RenderSystem->EndFrame();

        m_Window->SwapBuffers();

        if (m_Window->ShouldClose())
            m_Running = false;
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


void Engine::Render()
{
    m_RenderSystem->BeginFrame();
 
    m_RenderSystem->EndFrame();
}

void Engine::Shutdown()
{
    m_RenderSystem.reset();
    m_JobSystem.reset();
    m_InputSystem.reset();
    m_Window.reset();
}