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
    public:
        struct Desc
        {
            int         width = 1600;
            int         height = 900;
            std::string title = "Engine";
            bool        vsync = true;
            bool        resizable = true;
        };

        explicit Window(const Desc& desc);
        ~Window();

        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;
        Window(Window&&) = delete;
        Window& operator=(Window&&) = delete;

        [[nodiscard]] bool ShouldClose() const noexcept;
        void PollEvents() noexcept;
        void SwapBuffers() const noexcept;

        [[nodiscard]] GLFWwindow* NativeHandle() const noexcept { return m_Handle; }
        [[nodiscard]] int  Width()   const noexcept { return m_Width; }
        [[nodiscard]] int  Height()  const noexcept { return m_Height; }

        [[nodiscard]] float Aspect() const noexcept
        {
            return static_cast<float>(m_Width) / static_cast<float>(m_Height);
        }

        using ResizeFn = void(*)(void*, int, int);

        void SetResizeCallback(void* userData, ResizeFn fn) noexcept;

    private:
        static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

        GLFWwindow* m_Handle         = nullptr;
        int         m_Width          = 0;
        int         m_Height         = 0;
        bool        m_Vsync          = true;
        bool        m_Resizable      = true;
        void*       m_ResizeUser     = nullptr;
        ResizeFn    m_ResizeCallback = nullptr;
    };
}
