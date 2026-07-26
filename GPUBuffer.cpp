#include "GPUBuffer.hpp"
#include <iostream>

namespace Rendering
{
    GPUBuffer::GPUBuffer(GPUBuffer&& other) noexcept
    : m_VAO(std::exchange(other.m_VAO, 0))
    , m_VBO(std::exchange(other.m_VBO, 0))
    , m_IBO(std::exchange(other.m_IBO, 0))
    , m_IndexCount(std::exchange(other.m_IndexCount, 0))
{}

    GPUBuffer& GPUBuffer::operator=(GPUBuffer&& other) noexcept
    {
        if (this != &other)
        {
                // release current
            if (m_IBO)  
                glDeleteBuffers(1, &m_IBO);
            
            if (m_VBO)  
                glDeleteBuffers(1, &m_VBO);
            
            if (m_VAO)  
                glDeleteVertexArrays(1, &m_VAO);

                // take ownership
            m_VAO = std::exchange(other.m_VAO, 0);
            m_VBO = std::exchange(other.m_VBO, 0);
            m_IBO = std::exchange(other.m_IBO, 0);
            m_IndexCount = std::exchange(other.m_IndexCount, 0);
        }
        return *this;
    }

    GPUBuffer::~GPUBuffer()
    {
        if (m_IBO) 
            glDeleteBuffers(1, &m_IBO);
        
        if (m_VBO) 
            glDeleteBuffers(1, &m_VBO);
        
        if (m_VAO) 
            glDeleteVertexArrays(1, &m_VAO);
    }

    void GPUBuffer::Upload(const void* vertexData, std::size_t vertexBytes, 
                          const void* indexData,  std::size_t indexBytes, 
                          uint32_t vertexCount, uint32_t indexCount, BufferUsage usage)
    {
        assert(vertexData != nullptr && vertexBytes > 0 && indexData != nullptr && indexBytes > 0);

        // Create VAO/VBO/IBO if not already created
        if (m_VAO == 0) 
            glGenVertexArrays(1, &m_VAO);
        
        if (m_VBO == 0) 
            glGenBuffers(1, &m_VBO);
        
        if (m_IBO == 0) 
            glGenBuffers(1, &m_IBO);

        glBindVertexArray(m_VAO);

            // VBO
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertexBytes), vertexData, static_cast<GLenum>(usage));

            // IBO (element array)
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indexBytes), indexData, static_cast<GLenum>(usage));

            // Leave VAO bound; attribute pointers should have been configured by caller via ConfigureDefaultAttributes<T>()
        glBindVertexArray(0);

        m_IndexCount = static_cast<GLsizei>(indexCount);
    }


    void GPUBuffer::UpdateVertexData(const void* vertexData, std::size_t vertexBytes)
    {
        if (m_VBO == 0) 
            return; // or assert


        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, static_cast<GLsizeiptr>(vertexBytes), vertexData);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void GPUBuffer::UpdateIndexData(const void* indexData, std::size_t indexBytes)
    {
        if (m_IBO == 0)
            return; // or assert
            

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, static_cast<GLsizeiptr>(indexBytes), indexData);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }


    void GPUBuffer::Bind() const
    {
        glBindVertexArray(m_VAO);
    }

    void GPUBuffer::Unbind()
    {
        glBindVertexArray(0);
    }
}
