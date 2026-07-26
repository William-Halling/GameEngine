#include "GPUBuffer.h"
#include <iostream>

namespace Rendering
{
    GPUBuffer::~GPUBuffer()
    {
        if (m_IBO) 
            glDeleteBuffers(1, &m_IBO);
        
        if (m_VBO) 
            glDeleteBuffers(1, &m_VBO);
        
        if (m_VAO) 
            glDeleteVertexArrays(1, &m_VAO);
    }

    void GPUBuffer::Upload(const void* vertexData, std::size_t vertexCount, std::size_t vertexStride, const std::vector<uint32_t>& indices)
    {
        m_IndexCount = static_cast<GLsizei>(indices.size());

        // Create VAO
        glGenVertexArrays(1, &m_VAO);
        glBindVertexArray(m_VAO);

        // Create VBO
        glGenBuffers(1, &m_VBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, vertexCount * vertexStride, vertexData, GL_STATIC_DRAW);

        // Create IBO
        glGenBuffers(1, &m_IBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

           
            // Position (vec3)
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexStride, (void*)0);

            // Normal (vec3)
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexStride, (void*)(sizeof(float) * 3));

            // UV (vec2)
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexStride, (void*)(sizeof(float) * 6));

        glBindVertexArray(0);
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
