#include "Mesh.hpp"
#include <cassert>

namespace Rendering
{
    void Mesh::Initialize(std::span<const Vertex> vertices, std::span<const uint32_t> indices)
    {
        if (vertices.empty() || indices.empty())
        {
            assert(!vertices.empty() && !indices.empty());
     
            return;
        }

        m_VertexCount = static_cast<uint32_t>(vertices.size());
        m_IndexCount  = static_cast<uint32_t>(indices.size());

            // GPUBuffer::Upload signature expected:
            // Upload(const void* vData, size_t vBytes, const void* iData, size_t iBytes, uint32_t vCount, uint32_t iCount)
        m_Buffer.Upload(vertices.data(), sizeof(Vertex) * vertices.size(), indices.data(),  sizeof(uint32_t) * indices.size(), m_VertexCount, m_IndexCount);
    }

    void Mesh::UpdateVertices(std::span<const Vertex> vertices) noexcept
    {
        (void)UpdateData(vertices, m_VertexCount, [this](const void* ptr, size_t bytes) { m_Buffer.UpdateVertexData(ptr, bytes); });
    }

    void Mesh::UpdateIndices(std::span<const uint32_t> indices) noexcept
    {
        (void)UpdateData(indices, m_IndexCount, [this](const void* ptr, size_t bytes) { m_Buffer.UpdateIndexData(ptr, bytes); });
    }

    bool Mesh::TryUpdateVertices(std::span<const Vertex> vertices) noexcept
    {
        return UpdateData(vertices, m_VertexCount, [this](const void* ptr, size_t bytes) { m_Buffer.UpdateVertexData(ptr, bytes); });
    }

    bool Mesh::TryUpdateIndices(std::span<const uint32_t> indices) noexcept
    {
        return UpdateData(indices, m_IndexCount, [this](const void* ptr, size_t bytes) { m_Buffer.UpdateIndexData(ptr, bytes); });
    }

    void Mesh::Bind() const noexcept
    {
        m_Buffer.Bind();
    }

    void Mesh::Unbind() noexcept
    {
        GPUBuffer::Unbind();
    }
}