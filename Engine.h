#pragma once

#include <chrono>
#include <memory>
#include <vector>

namespace Core      { class Window; class InputSystem; class JobSystem; }
namespace Rendering { class RenderSystem; }
namespace Game      { namespace NPC { struct NPCComponent; struct NPCBlackboard; } }
namespace Scripting { class LuaVM; }
namespace Gameplay  { class CommandBuffer; }
namespace Game      { struct CameraData; }


class Engine
{
    public:
        Engine();
        ~Engine();

        Engine(const Engine&) = delete;
        Engine& operator=(const Engine&) = delete;

        void Run();

    private:
        void Update(float dt);
        void Render();
        void Shutdown();
        
        std::unique_ptr<Core::Window>        m_Window;
        std::unique_ptr<Core::InputSystem>   m_InputSystem;
        std::unique_ptr<Core::JobSystem>     m_JobSystem;
        std::unique_ptr<Scripting::LuaVM>    m_Lua;
        std::unique_ptr<Rendering::RenderSystem> m_RenderSystem;

        // Game data
        std::vector<Game::NPC::NPCComponent> m_NPCs;
        Game::NPC::NPCBlackboard             m_NPCBlackboard;
        Game::CameraData                     m_Camera;
        std::unique_ptr<Gameplay::CommandBuffer> m_Commands;

        // Timing
        std::chrono::steady_clock::time_point m_LastTime;
        bool m_Running = true;
};