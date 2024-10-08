#pragma once

#include "Dix/Core/Base.h"

namespace Dix
{
	class Shader
	{
	public:
		Shader(const std::string& filepath);
		~Shader();

		void Bind() const;
		void Unbind() const;

		std::string GetName() const { return m_Name; }
		std::string GetFilepath() const { return m_Filepath; }

		void SetVec(const std::string& name, const glm::vec3& vec);
		void SetVec(const std::string& name, const glm::vec4& vec);

		void SetMat(const std::string& name, const glm::mat3& mat);
		void SetMat(const std::string& name, const glm::mat4& mat);

		static SharedPtr<Shader> Create(const std::string& filepath);

	private:
		std::string ReadFile();
		std::unordered_map<u32, std::string> Preprocess(const std::string& source);
		void CompileProgram(std::unordered_map<u32, std::string>& sources);

	private:
		u32 m_RendererID = 0;

		std::string m_Filepath, m_Name;
	};
}