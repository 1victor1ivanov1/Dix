#pragma once

#include "Dix/Core/Base.h"

#include "Dix/Scene/Mesh.h"

namespace Dix
{
	class Model
	{
	public:
		Model(const std::string& filepath);
		~Model();

		const std::string& GetPath() const { return m_Path; }
		const std::string& GetName() const { return m_Name; }

		void Render() const;

		static SharedPtr<Model> Create(const std::string filepath);

	private:
		std::vector<SharedPtr<Mesh>> m_Meshes;

		std::string m_Path, m_Name;
	};
}