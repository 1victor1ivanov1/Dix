#include "dixpch.h"

#include "Dix/Renderer/Shader.h"

#include <glad/glad.h>

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

		size_t lastSlash = filepath.find_last_of("/\\");
		size_t lastDot = filepath.rfind(".");
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
			size_t length = file.tellg();

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
	std::unordered_map<uint32_t, std::string> Dix::Shader::Preprocess(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> result;

		const char* typeToken = "#type";
		size_t typeTokenLen = strlen(typeToken);
		size_t pos = source.find(typeToken);

		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			DIX_CORE_ASSERT(eol != std::string::npos, "Syntax error!");
			size_t begin = pos + typeTokenLen + 1;
			std::string type = source.substr(begin, eol - begin);
			DIX_CORE_ASSERT(Utils::ShaderTypeFromString(type), "Invalid shader type specified!");

			size_t nextLinePos = source.find_first_of("\r\n", eol);
			DIX_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error!");
			pos = source.find(typeToken, nextLinePos);

			result[Utils::ShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
		}

		return result;
	}
	void Dix::Shader::CompileProgram(std::unordered_map<GLenum, std::string>& sources)
	{
		uint32_t program = glCreateProgram();

		std::vector<uint32_t> shaderIDs;
		for (auto& [type, source] : sources)
		{
			const char* sourceCode = source.c_str();
			uint32_t shaderID = shaderIDs.emplace_back(glCreateShader(type));
			glShaderSource(shaderID, 1, &sourceCode, NULL);
			glCompileShader(shaderID);

			int status;
			char infoLog[1024];
			glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
			if (status == GL_FALSE)
			{
				glGetShaderInfoLog(shaderID, 1024, nullptr, infoLog);
				DIX_CORE_ERROR("Shader compiling error ({0}): {1}", m_Filepath, infoLog);
			}
			glAttachShader(program, shaderID);
		}

		glLinkProgram(program);

		int status;
		char infoLog[1024];
		glGetProgramiv(program, GL_LINK_STATUS, &status);
		if (status == GL_FALSE)
		{
			glGetProgramInfoLog(program, 1024, nullptr, infoLog);
			DIX_CORE_ERROR("Shader linking error ({0}): {1}", m_Filepath, infoLog);
			glDeleteProgram(program);
		}

		for (auto shaderID : shaderIDs)
		{
			glDetachShader(program, shaderID);
			glDeleteShader(shaderID);
		}

		m_RendererID = program;
	}
}