#pragma once

#include "Dix/Core/Base.h"

#include "Dix/Scene/Mesh.h"

namespace Dix
{
	class Model
	{
	public:
		Model(const std::vector<SharedPtr<Mesh>>& meshes);
		~Model();

		void Render() const;

		static SharedPtr<Model> Create(const std::vector<SharedPtr<Mesh>>& meshes);

	private:
		std::vector<SharedPtr<Mesh>> m_Meshes;
	};
}