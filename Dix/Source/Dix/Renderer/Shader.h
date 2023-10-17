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

		static SharedPtr<Shader> Create(const std::string& filepath);

	private:
		std::string ReadFile();
		std::unordered_map<uint32_t, std::string> Preprocess(const std::string& source);
		void CompileProgram(std::unordered_map<uint32_t, std::string>& sources);

	private:
		uint32_t m_RendererID = 0;

		std::string m_Filepath, m_Name;
	};
}