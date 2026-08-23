#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

namespace Rendering
{
	class Shader
	{
	public:
		Shader() = default;
		~Shader();

		Shader(const Shader&) = default;
		Shader& operator=(const Shader&) = delete;

		bool LoadFromFiles(const std::string& vertexPath, const std::string& fragmentPath);

		void Bind() const;
		static void Unbind();

		void SetUniform(const std::string& name, int value) const;
		void SetUniform(const std::string& name, float value) const;
		void SetUniform(const std::string& name, const glm::vec3& value) const;
		void SetUniform(const std::string& name, const glm::mat4& value) const;

	private:
		unsigned int m_Program = 0;
		mutable std::unordered_map<std::string, int> m_UniformCache;

		int GetUniformLocation(const std::string& name) const;
		static std::string ReadFile(const std::string& path);
		static unsigned int CompileStage(unsigned int type, const std::string& source);
	};
}

#endif