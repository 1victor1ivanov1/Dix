#include "dixpch.h"

#include "ModelImporter.h"

#include <TinyObjLoader/tiny_obj_loader.h>

namespace Dix
{
    SharedPtr<Model> Dix::ModelImporter::LoadObjModel(const std::filesystem::path& filepath)
    {
        tinyobj::ObjReader reader;

        if (!reader.ParseFromFile(filepath.string()))
        {
            DIX_CORE_ERROR("Failed to load obj file({0}): {1}", filepath, reader.Error());
            DIX_CORE_ASSERT(false);
        }

        if (!reader.Warning().empty())
        {
            DIX_CORE_WARN("Warning while reading obj file({0}): {1}", filepath, reader.Warning());
        }

        auto& attrib = reader.GetAttrib();
        auto& shapes = reader.GetShapes();
        auto& materials = reader.GetMaterials();

        std::vector<MeshVertex> vertexes;
        std::unordered_map<MeshVertex, u32> uniqueVertexes;
        std::vector<u32> indices;

        std::vector<SharedPtr<Mesh>> meshes;

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

                vertex.Normal = {
                    attrib.normals[3 * index.normal_index + 0],
                    attrib.normals[3 * index.normal_index + 1],
                    attrib.normals[3 * index.normal_index + 2],
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

        meshes.push_back(Mesh::Create(indices, vertexes));

        return Model::Create(meshes);
    }

    SharedPtr<Model> ModelImporter::LoadGLTFModel(const std::filesystem::path& filepath)
    {
        return SharedPtr<Model>();
    }
}