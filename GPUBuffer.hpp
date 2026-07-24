#pragma once
#include <glad/glad.h>
#include <vector>
#include <cstdint>

namespace Rendering
{
	class GPUBuffer
	{
		GPUBuffer() = default;
		~GPUBuffer();

		GPUBuffer(const GPUBuffer&) = delete;
		GPUBuffer& operator=(const GPUBuffer&) = delete;

		void Upload(const void* vertexData, std::size_t vertexCount, std::size_t vertexStride, const std::vector<uint32_t>& indicies);
		void Bind() const;
		static void Unbind();

		[[nodiscard]] GLsizei IndexCount() const noexcept { return m_IndexCount; }

	private:
		GLuint m_VAO = 0;
		GLuint m_VBO = 0;
		GLuint m_IBO = 0;

		GLsizei m_IndexCount = 0;
	};
}