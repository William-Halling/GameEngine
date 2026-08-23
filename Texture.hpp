#pragma once
#include <glad/glad.h>
#include <string>
#include <cstdint>

namespace Rendering
{
    enum class TextureFormat : GLenum
    {
        RBG = GL_RGB,

        RBGA = GL_RGBA
    };

    enum class TextureFilter : GLint
    {
        Nearest = GL_NEAREST,

        Linear = GL_LINEAR
    }
    

    

}