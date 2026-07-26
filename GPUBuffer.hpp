#pragma once

#include <glad/glad.h>
#include <cstddef>      // offsetof
#include <type_traits>  // is_standard_layout
#include <utility>
#include <cstdint>

namespace Rendering
{
	enum class BufferUsage : GLenum
	{
		Static  = GL_STATIC_DRAW,
		Dynamic = GL_DYNAMIC_DRAW,
		Stream  = GL_STREAM_DRAW
	};

	class GPUBuffer
	{
		public:
			
			GPUBuffer() = default;
			~GPUBuffer();

			GPUBuffer(const GPUBuffer&) = delete;
			GPUBuffer& operator=(const GPUBuffer&) = delete;

			GPUBuffer(GPUBuffer&&) noexcept;
			GPUBuffer& operator=(GPUBuffer&&) noexcept;

			void Upload(const void* vertexData, std::size_t vertexBytes, const void* indexData, std::size_t indexBytes, uint32_t vertexCount, uint32_t indexCount, BufferUsage usage = BufferUsage::Static);
			void UpdateVertexData(const void* vertexData, std::size_t vertexBytes, std::size_t offsetBytes = 0);
			void UpdateIndexData(const void* indexData, std::size_t indexBytes, std::size_t offsetBytes = 0);

			void Bind() const noexcept;
			static void Unbind() noexcept;

			[[nodiscard]] GLsizei IndexCount() const noexcept { return m_IndexCount; }

			/**
			 * @brief Configure attribute layout for a vertex type T. Must be called before Upload.
			 * @brief This is a convenience helper that sets up three attributes: position, normal, uv.
			 */
			template<typename VertexT>
			static void ConfigureDefaultAttributes();
		
		private:
			GLuint m_VAO = 0;
			GLuint m_VBO = 0;
			GLuint m_IBO = 0;

			GLsizei m_IndexCount = 0;
	};

	template<typename VertexT>
	void GPUBuffer::ConfigureDefaultAttributes()
	{
		static_assert(std::is_standard_layout<VertexT>::value, "VertexT must be standard layout");
		constexpr GLsizei stride = static_cast<GLsizei>(sizeof(VertexT));

		// Position at location 0
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<const void*>(offsetof(VertexT, position)));

		// Normal at location 1
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<const void*>(offsetof(VertexT, normal)));

		// UV at location 2
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<const void*>(offsetof(VertexT, uv)));
	}
}