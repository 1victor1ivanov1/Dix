#include "dixpch.h"

#include "Dix/Renderer/Shader.h"

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

namespace Dix
{
	namespace Utils
	{
		GLenum ShaderTypeFromString(const std::string& type)
		{
			if (type ==  "vertex")
				return GL_VERTEX_SHADER;
			else if (type == "fragment")
				return GL_FRAGMENT_SHADER;
			else if (type == "geometry")
				return GL_GEOMETRY_SHADER;
			else if (type == "compute")
				return GL_COMPUTE_SHADER;

			DIX_CORE_ASSERT(false, "Unknown shader type!");
			return 0;
		}
	}

	Shader::Shader(const std::string& filepath)
		: m_Filepath(filepath)
	{
		auto source = ReadFile();
		auto shaderSources = Preprocess(source);
		CompileProgram(shaderSources);

		u64 lastSlash = filepath.find_last_of("/\\");
		u64 lastDot = filepath.rfind(".");
		m_Name = filepath.substr(lastSlash + 1, lastDot - lastSlash - 1);
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_RendererID);
	}

	void Shader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void Shader::Unbind() const
	{
		glUseProgram(0);
	}

	void Shader::SetMat(const std::string& name, const glm::mat3& mat)
	{
		glUniformMatrix3fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
	}

	void Shader::SetMat(const std::string& name, const glm::mat4& mat)
	{
		glUniformMatrix4fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
	}

	SharedPtr<Shader> Shader::Create(const std::string& filepath)
	{
		return MakeShared<Shader>(filepath);
	}

	std::string Shader::ReadFile()
	{
		std::string result;
		std::ifstream file(m_Filepath, std::ios::in | std::ifstream::binary);

		if (file)
		{
			file.seekg(0, std::ios::end);
			u64 length = file.tellg();

			if (length != -1)
			{
				result.resize(length);
				file.seekg(0, std::ios::beg);
				file.read(&result[0], length);
			}
			else
			{
				DIX_CORE_ERROR("Failed to read from file at {0}", m_Filepath);
			}
		}
		else
		{
			DIX_CORE_ERROR("Failed to load shader file at {0}", m_Filepath);
		}

		return result;
	}
	std::unordered_map<u32, std::string> Dix::Shader::Preprocess(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> result;

		const char* typeToken = "#type";
		u64 typeTokenLen = strlen(typeToken);
		u64 pos = source.find(typeToken);

		while (pos != std::string::npos)
		{
			u64 eol = source.find_first_of("\r\n", pos);
			DIX_CORE_ASSERT(eol != std::string::npos, "Syntax error!");
			u64 begin = pos + typeTokenLen + 1;
			std::string type = source.substr(begin, eol - begin);
			DIX_CORE_ASSERT(Utils::ShaderTypeFromString(type), "Invalid shader type specified!");

			u64 nextLinePos = source.find_first_of("\r\n", eol);
			DIX_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error!");
			pos = source.find(typeToken, nextLinePos);

			result[Utils::ShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
		}

		return result;
	}
	void Dix::Shader::CompileProgram(std::unordered_map<u32, std::string>& sources)
	{
		u32 program = glCreateProgram();

		std::vector<u32> shaders;
		for (auto& [type, source] : sources)
		{
			const char* sourceCode = source.c_str();
			u32 shader = shaders.emplace_back(glCreateShader(type));
			glShaderSource(shader, 1, &sourceCode, NULL);
			glCompileShader(shader);

			i32 status;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
			if (status == GL_FALSE)
			{
				i32 length;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
				std::vector<GLchar >infoLog(length);
				glGetShaderInfoLog(shader, length, &length, infoLog.data());
				DIX_CORE_ERROR("Shader compiling error ({0}): {1}", m_Filepath, infoLog.data());
			}
			glAttachShader(program, shader);
		}

		glLinkProgram(program);

		i32 status;
		glGetProgramiv(program, GL_LINK_STATUS, &status);
		if (status == GL_FALSE)
		{
			i32 length;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
			std::vector<GLchar >infoLog(length);
			glGetProgramInfoLog(program, 1024, nullptr, infoLog.data());
			DIX_CORE_ERROR("Shader linking error ({0}): {1}", m_Filepath, infoLog.data());
			glDeleteProgram(program);
		}

		for (auto shader : shaders)
		{
			glDetachShader(program, shader);
			glDeleteShader(shader);
		}

		m_RendererID = program;
	}
}