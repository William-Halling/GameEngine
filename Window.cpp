#include "Window.h"
#include <glad/glad.h>
#include <stdexcept>
#include <iostream>

namespace Core
{
    static void SetGLFWHints(const Window::Desc& desc)
    {
        glfwWindowHint(GLFW_RESIZABLE, desc.resizable ? GLFW_TRUE : GLFW_FALSE);
    }

    Window::Window(const Desc& desc)
    {
        if (!glfwInit())
            return;

        SetGLFWHints(desc);

        m_Handle = glfwCreateWindow(desc.width, desc.height, desc.title.c_str(), nullptr, nullptr);

        if (!m_Handle)
        {
            glfwTerminate();
            return;
        }

        m_Width = desc.width;
        m_Height = desc.height;
        m_Vsync = desc.vsync;
        m_Resizable = desc.resizable;

        glfwMakeContextCurrent(m_Handle);
        glfwSwapInterval(m_Vsync ? 1 : 0);

        glfwSetWindowUserPointer(m_Handle, this);
        glfwSetFramebufferSizeCallback(m_Handle, FramebufferSizeCallback);
    }

    Window::~Window()
    {
        if (m_Handle)
        {
            glfwDestroyWindow(m_Handle);
            glfwTerminate();
        }
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

    void Window::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
    {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
        
        if (!self)
            return;

        self->m_Width  = width;
        self->m_Height = height;
        glViewport(0, 0, width, height);

        if (self->m_ResizeCallback)
            self->m_ResizeCallback(self->m_ResizeUser, width, height);
    }
}