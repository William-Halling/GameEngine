#ifndef RENDERING_MESH_HPP
#define RENDERING_MESH_HPP

#include <cstdint>
#include <span>
#include <cassert>
#include <glm/glm.hpp>
#include "GPUBuffer.hpp"

namespace Rendering
{
    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 uv;
    };


/**************************************
 * Mesh
 *
 * CPU-side handle for GPU geometry. Owns a GPUBuffer (VAO/VBO/IBO) and
 * provides a thin, RAII-friendly API for uploading and binding geometry.
 *
 * Public API uses std::span for zero-cost, flexible views over caller data.
 **************************************/

    class Mesh
    {
        public:
            Mesh() = default;
            ~Mesh() = default;

                ///< Prevent accidental copies of GPU resources.
            Mesh(const Mesh&) = delete; 
            Mesh& operator=(const Mesh&) = delete;

                ///< Enable move semantiics
            Mesh(Mesh&&) noexcept = default;
            Mesh& operator=(Mesh&&) noexcept = default;

                /**
                 * @brief Initialize
                 * @brief Uploads vertex and index arrays to the GPU and configures the VAOs.
                 */
            void Initialize(std::span<const Vertex> vertices, std::span<const uint32_t> indices);

                /**
                 * @brief UpdateVerticies / UpdateIndices
                 * @brief Non-throwing convenience wrappers. Use these when vertex/index count is stable.
                 * @brief For callers that need feedback, use TryUpdate* which returns bool
                 */
            void UpdateVertices(std::span<const Vertex> vertices) noexcept;
            void UpdateIndices(std::span<const uint32_t> indices) noexcept;

                /** 
                 * @brief TryUpdate returns false if the update failed (count mismatch). 
                 */
            bool TryUpdateVertices(std::span<const Vertex> vertices) noexcept;
            bool TryUpdateIndices(std::span<const uint32_t> indices) noexcept;
            
                /**
                 * @brief Binds the mesh's VAO and associated buffers so the GPU is ready to draw.
                 */
            void Bind() const noexcept;

                /**
                 * @brief Unbind any mesh-specific GPU state
                 */
            static void Unbind() noexcept;

                /**
                 * @brief Get the number of indices to draw  
                 */
            uint32_t GetIndexCount() const noexcept { return m_IndexCount; }

                /** 
                 * @brief Get the number of vertices stored.
                 */
            uint32_t GetVertexCount() const noexcept { return m_VertexCount; }

                /** 
                 * @brief Checks and returns true if the mesh has valid GPU data uploaded. 
                 */
            bool IsValid() const noexcept { return m_IndexCount != 0 && m_VertexCount != 0; }

        private:
            template<typename T, typename Updater>
            bool UpdateData(std::span<const T> data, uint32_t currentCount, Updater updater) noexcept;
            
            GPUBuffer m_Buffer;                 ///< RAII GPU buffer wrapper (VAO/VBO/IBO).
            uint32_t  m_VertexCount = 0;        ///< number of vertices currently stored.
            uint32_t  m_IndexCount  = 0;        ///< number of indices currently stored.
    }


    template<typename T, typename Updater>
    bool Mesh::UpdateData(std::span<const T> data, uint32_t currentCount, Updater updater) noexcept
    {
        if (data.empty())
            return true;

        uint32_t newCount = static_cast<uint32_t>(data.size());

        assert(newCount == currentCount && "UpdateData requires stable element count; reinitialize instead.");

        if (newCount != currentCount)
            return false;

        updater(static_cast<const void*>(data.data()), sizeof(T) * data.size());

        return true;
    }
};
#endif