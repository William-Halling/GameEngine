#pragma once
#include <string>
#include <functional>
#include <GLFW/glfw3.h>

/******************************************


 ******************************************/
namespace Core
{
    class Window
    {
        struct Desc
        {
            int         width     = 1600;
            int         height    = 900;
            std::string title     = "Engine";
            bool        vsync     = true;
            bool        resizable = true;
        };

        explicit Window(const Desc& desc);
        ~Window();

        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;
        Window(Window&&) = delete;
        Window& operator=(Window&&) = delete;

            // Create the native window and make its GL context current.
            // Throws std::runtime_error on failure.
        void Init();


            // Poll OS events (call on main thread each frame).
        void PollEvents() noexcept;


            // Swap front/back buffers (call on main thread after rendering).
        void SwapBuffers() const noexcept;


            // Query whether the user requested close.
        [[nodiscard]] bool ShouldClose() const noexcept;


            // Native handle for advanced usage (do not destroy externally).
        [[nodiscard]] GLFWwindow* NativeHandle() const noexcept { return m_Handle; }


        [[nodiscard]] int Width()  const noexcept { return m_Width; }
        [[nodiscard]] int Height() const noexcept { return m_Height; }


        [[nodiscard]] float Aspect() const noexcept
        {
            return static_cast<float>(m_Width) / static_cast<float>(m_Height);
        }

            // Resize callback: userData is forwarded as-is. Callback runs on main thread.
        using ResizeFn = void(*)(void* userData, int width, int height);


            // Set a resize callback; safe to call before or after Init.
        void SetResizeCallback(void* userData, ResizeFn fn) noexcept;


            // Enable or disable VSync at runtime (main thread).
        void SetVSync(bool enabled) noexcept;

    private:
            // GLFW framebuffer callback forwards to instance method.
        static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);


            // Internal helper to update stored width/height and call user callback.
        void OnFramebufferSize(int width, int height) noexcept;

        
        GLFWwindow* m_Handle         = nullptr;
        int         m_Width          = 0;
        int         m_Height         = 0;
        bool        m_Vsync          = true;
        bool        m_Resizable      = true;

            // user-provided resize callback
        void*       m_ResizeUser     = nullptr;
        ResizeFn    m_ResizeCallback = nullptr;
    };
}
