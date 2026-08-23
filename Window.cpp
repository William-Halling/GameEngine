#include <glad/glad.h>
#include <stdexcept>
#include <iostream>
#include "Window.h"

namespace Core
{
    static void SetGLFWHints(const Window::Desc& desc)
    {
        // Request a modern core profile by default
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #if defined(__APPLE__)
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

        glfwWindowHint(GLFW_RESIZABLE, desc.resizable ? GLFW_TRUE : GLFW_FALSE);
    }


    Window::Window(const Desc& desc)
        : m_Width(desc.width)
        , m_Height(desc.height)
        , m_Vsync(desc.vsync)
        , m_Resizable(desc.resizable)
    {}


    Window::~Window()
    {
        if (m_Handle)
        {
            glfwDestroyWindow(m_Handle);
            m_Handle = nullptr;
        }
    }

    void Window::Init()
    {
        if (m_Handle)
            return; 

        if (!glfwInit())
            throw std::runtime_error("GLFW initialization failed");

        SetGLFWHints({ m_Width, m_Height, std::string("Engine"), m_Vsync, m_Resizable });

        m_Handle = glfwCreateWindow(m_Width, m_Height, "Engine", nullptr, nullptr);
        
        if (!m_Handle)
        {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }

            // Make context current on this thread
        glfwMakeContextCurrent(m_Handle);

            // Set swap interval (vsync)
        glfwSwapInterval(m_Vsync ? 1 : 0);

            // Set user pointer so static callbacks can forward to this instance
        glfwSetWindowUserPointer(m_Handle, this);
        glfwSetFramebufferSizeCallback(m_Handle, FramebufferSizeCallback);

           // Query actual framebuffer size
        int fbw = 0, fbh = 0;
        glfwGetFramebufferSize(m_Handle, &fbw, &fbh);
        m_Width = fbw;
        m_Height = fbh;
    }

    bool Window::ShouldClose() const noexcept
    {
        return !m_Handle || glfwWindowShouldClose(m_Handle);
    }

    void Window::PollEvents() noexcept
    {
        glfwPollEvents();
    }

    void Window::SwapBuffers() const noexcept
    {
        if (m_Handle)
            glfwSwapBuffers(m_Handle);
    }

    void Window::SetResizeCallback(void* userData, ResizeFn fn) noexcept
    {
        m_ResizeUser = userData;

        m_ResizeCallback = fn;
    }

    void Window::SetVSync(bool enabled) noexcept
    {
        m_Vsync = enabled;
     
        if (m_Handle)
            glfwSwapInterval(m_Vsync ? 1 : 0);
    }


    void Window::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
    {
        if (!window) 
            return;

        void* ptr = glfwGetWindowUserPointer(window);
            
        if (!ptr) 
            return;

        Window* self = reinterpret_cast<Window*>(ptr);
        self->OnFramebufferSize(width, height);
    }


    void Window::OnFramebufferSize(int width, int height) noexcept
    {
        m_Width = width;
        m_Height = height;

        glViewport(0, 0, width, height);

        if (m_ResizeCallback)
            m_ResizeCallback(m_ResizeUser, width, height);
    }
}