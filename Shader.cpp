#include "Shader.hpp"
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>

namespace Rendering
{
	Shader::~Shader()
	{
		if (m_Program != 0)
			glDeleteProgram(m_Program);
	}

	std::string Shader::ReadFile(const std::string& path)
	{
		std::ifstream file(path);

		if (!file.is_open())
		{
			std::cerr << "Failed to open shader file: " << path << "\n";

			return{};
		}

		std::stringstream ss;
		ss << file.rdbuf();

		return ss.str();
	}


	unsigned int Shader::CompileStage(unsigned int type, const std::string& source)
	{
		unsigned int shader = glCreateShader(type);
		const char* src = source.c_str();

		glShaderSource(shader, 1, &src, nullptr);
		glCompileShader(shader);

		int success = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			char log[1024];
			glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
			std::cerr << "Shader compile error :\n" << log << "\n";
			glDeleteShader(shader);

			return 0;
		}

		return shader;
	}

	bool Shader::LoadFromFiles(const std::string& vertexPath, const std::string& fragmentPath)
	{
		std::string vertSrc = ReadFile(vertexPath);
		std::string fragSrc = ReadFile(fragmentPath);

		if (vertSrc.empty() || fragSrc.empty())
			return false;

		unsigned int vert = CompileStage(GL_VERTEX_SHADER, vertSrc);
		unsigned int frag = CompileStage(GL_FRAGMENT_SHADER, fragSrc);

		if (vert == 0 || frag == 0)
		{
			if (vert) glDeleteShader(vert);

			if (frag) glDeleteShader(frag);

			return false;
		}

		unsigned int program = glCreateProgram();
		glAttachShader(program, vert);
		glAttachShader(program, frag);
		glLinkProgram(program);

		glDeleteShader(vert);
		glDeleteShader(frag);

		int success = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		
		if (!success)
		{
			char log[1024];
			glGetProgramInfoLog(program, sizeof(log), nullptr, log);
			std::cerr << "Program link error:\n" << log << "\n";
			glDeleteProgram(program);

			return false;
		}

		if (m_Program != 0)
			glDeleteProgram(m_Program);

		m_Program = program;
	
		return true;
	}

	void Shader::Bind() const
	{
		glUseProgram(m_Program);
	}

	void Shader::Unbind()
	{
		glUseProgram(0);
	}

	void Shader::SetUniform(const std::string& name, int value) const
	{
		int loc = GetUniformLocation(name);

		if (loc != -1)
			glUniform1i(loc, value);
	}

	void Shader::SetUniform(const std::string& name, float value) const
	{
		int loc = GetUniformLocation(name);

		if (loc != -1)
			glUniform1f(loc, value);
	}

	void Shader::SetUniform(const std::string& name, const glm::vec3& value) const
	{
		int loc = GetUniformLocation(name);

		if (loc != -1)
			glUniform3fv(loc, 1, &value[0]);
	}

	void Shader::SetUniform(const std::string& name, const glm::mat4& value) const
	{
		int loc = GetUniformLocation(name);

		if (loc != -1)
			glUniformMatrix4fv(loc, 1, GL_FALSE, &value[0][0]);
	}

	int Shader::GetUniformLocation(const std::string& name) const
	{
		auto it = m_UniformCache.find(name);

		if (it != m_UniformCache.end())
			return it->second;

		int loc = glGetUniformLocation(m_Program, name.c_str());
		m_UniformCache[name] = loc;

		return loc;
	}
}