#include "dixpch.h"

#include "Dix/Scene/Model.h"



namespace Dix
{
    Model::Model(const std::vector<SharedPtr<Mesh>>& meshes)
        : m_Meshes(std::move(meshes))
	{
	}

	Model::~Model()
	{
	}
	
	void Model::Render() const
	{
		for (auto & mesh : m_Meshes)
			mesh->Render();
	}

	SharedPtr<Model> Model::Create(const std::vector<SharedPtr<Mesh>>& meshes)
	{
		return MakeShared<Model>(meshes);
	}
}