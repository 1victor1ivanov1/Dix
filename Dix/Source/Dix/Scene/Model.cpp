#include "dixpch.h"

#include "Dix/Scene/Model.h"

#include <TinyObjLoader/tiny_obj_loader.h>

namespace Dix
{
	Model::Model(const std::string& filepath)
	{
        tinyobj::ObjReader reader;

        if (!reader.ParseFromFile(filepath)) {
            DIX_CORE_ERROR("Failed to load obj file({0}): {1}", filepath, reader.Error());
            DIX_CORE_ASSERT(false);
        }

        if (!reader.Warning().empty()) {
            DIX_CORE_WARN("Warning while reading obj file({0}): {1}", filepath, reader.Warning());
        }

        auto& attrib = reader.GetAttrib();
        auto& shapes = reader.GetShapes();
        auto& materials = reader.GetMaterials();

        std::vector<MeshVertex> vertexes;
        std::vector<u32> indices;

        std::unordered_map<MeshVertex, u32> uniqueVertexes;

        for (const auto& shape : shapes)
        {
            for (const auto& index : shape.mesh.indices)
            {
                MeshVertex vertex{};

                vertex.Position = {
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]
                };

                vertex.TexCoord = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    attrib.texcoords[2 * index.texcoord_index + 1]
                };

                if (uniqueVertexes.count(vertex) == 0)
                {
                    uniqueVertexes[vertex] = uniqueVertexes.size();
                    vertexes.push_back(vertex);
                }

                indices.push_back(uniqueVertexes[vertex]);
            }
        }

        DIX_CORE_ASSERT(vertexes.size() > 0, "Array of vertexes is empty!");
        DIX_CORE_ASSERT(indices.size() > 0, "Array of indices is empty!");

        m_Meshes.push_back(Mesh::Create(indices, vertexes));
	}

	Model::~Model()
	{
	}
	
	void Model::Render() const
	{
		for (auto & mesh : m_Meshes)
			mesh->Render();
	}

	SharedPtr<Model> Model::Create(const std::string filepath)
	{
		return MakeShared<Model>(filepath);
	}
}