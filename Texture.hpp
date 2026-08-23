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
    

    enum class TextureWrap : GLint
    {
        Repeat = GL_REPEAT,

        ClampToEdge = GL_CLAMP_TO_EDGE,

        ClampToBorder = GL_CLAMP_TO_BORDER
    };

    enum TextureFlags : uint32_t
    {
        TextureFlag_None         = 0,
        TextureFlag_GenerateMips = 1 << 0,
        TextureFlag_Immutable    = 1 << 1,
        TextureFlag_sRGB         = 1 << 2
    };


    class Texture
    {
        public:
            Texture() = default;
            ~Texture();

            Texture(Texture&& other) noexcept;
            Texture& operator=(Texture&& other) noexcept;

            bool LoadFromFile(const std::string& path, TextureFormat format = TextureFormat::RBGA, TextureFilter filter = TextureFilter::Linear, TextureWrap wrap = TextureWrap::Repeat, 
                    uint32_t flags = TextureFlag_GenerateMips);

            
            bool LoadFromMemory(const void* pixels, int width, int height, int channels, TextureFormat format = TextureFormat::RGBA,
                    TextureFilter filter = TextureFilter::Linear, TextureWrap wrap = TextureWrap::Repeat, uint32_t flags = TextureFlag_GenerateMips);


                // Low level upload for main thread. Use for async decode workflows.
            void Upload2D(GLenum internalFormat, GLenum format, GLenum type, const void* pixels, int width, int height, bool generateMips, bool immutable);

                // Bind texture to unit
            void Bind(uint32_t unit = 0) const noexcept;

                // Unbind texture from active unit
            static void Unbind(uint32_t unit = 0) noexcept;

                // Debug helpers
            void SetDebugName(const char* name) noexcept;
            size_t EstimatedGPUMemory() const noexcept { return m_EstimatedSize; }

            [[nodiscard]] GLuint Handle() const noexcept { return m_Handle; }
            [[nodiscard]] int Width()     const noexcept { return m_Width;  }
            [[nodiscard]] int Height()    const noexcept { return m_Height; }
    
        private:
            GLuint m_Handle = 0;
            int    m_Width  = 0;
            int    m_Height = 0;
            GLenum m_InternalFormat = GL_RGBA8;
            size_t m_EstimatedSize = 0;

                // helpers
            static GLenum ChooseInternalFormat(TextureFormat fmt, bool srgb) noexcept;
            static int    ComputeMipLevels(int width, int height) noexcept;
    };

}