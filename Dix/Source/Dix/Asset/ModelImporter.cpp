#include "dixpch.h"

#include "ModelImporter.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Dix
{
    namespace Utils
    {
        static SharedPtr<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene)
        {
            std::vector<MeshVertex> vertexes;
            std::vector<u32> indices;

            for (u32 i = 0; i < mesh->mNumVertices; ++i)
            {
                MeshVertex vertex{};

                glm::vec3 vector;
                vector.x = mesh->mVertices[i].x;
                vector.y = mesh->mVertices[i].y;
                vector.z = mesh->mVertices[i].z;
                vertex.Position = vector;

                if (mesh->HasNormals())
                {
                    vector.x = mesh->mNormals[i].x;
                    vector.y = mesh->mNormals[i].y;
                    vector.z = mesh->mNormals[i].z;
                    vertex.Normal = vector;
                }
                if (mesh->mTextureCoords[0])
                {
                    glm::vec2 vec;
                    vec.x = mesh->mTextureCoords[0][i].x;
                    vec.y = mesh->mTextureCoords[0][i].y;
                    vertex.TexCoord = vec;

                    vector.x = mesh->mTangents[i].x;
                    vector.y = mesh->mTangents[i].y;
                    vector.z = mesh->mTangents[i].z;
                    vertex.Tangent = vector;

                    vector.x = mesh->mBitangents[i].x;
                    vector.y = mesh->mBitangents[i].y;
                    vector.z = mesh->mBitangents[i].z;
                    vertex.Bitangent = vector;
                }
                else
                {
                    vertex.TexCoord = glm::vec2(0.0f);
                    vertex.Tangent = glm::vec3(0.0f);
                    vertex.Bitangent = glm::vec3(0.0f);
                }

                vertexes.push_back(vertex);
            }

            for (u32 i = 0; i < mesh->mNumFaces; ++i)
            {
                aiFace face = mesh->mFaces[i];
                for (u32 j = 0; j < face.mNumIndices; ++j)
                    indices.push_back(face.mIndices[j]);
            }

            DIX_CORE_ASSERT(indices.size() > 0 && vertexes.size() > 0, "Amount of indices and vertexes must be greater than 0!");

            return Mesh::Create(indices, vertexes);
        }

        static void ProcessNode(aiNode* node, const aiScene* scene, std::vector<SharedPtr<Mesh>>& meshes)
        {
            for (u32 i = 0; i < node->mNumMeshes; ++i)
            {
                aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
                meshes.push_back(Utils::ProcessMesh(mesh, scene));
            }

            for (u32 i = 0; i < node->mNumChildren; ++i)
            {
                Utils::ProcessNode(node->mChildren[i], scene, meshes);
            }
        }
    }

    SharedPtr<Model> Dix::ModelImporter::LoadModel(const std::filesystem::path& filepath)
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(filepath.string(), aiProcessPreset_TargetRealtime_MaxQuality);
      
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            DIX_CORE_ERROR("ModelImporter: Failed to load model at {0}", filepath);
            return nullptr;
        }

        std::vector<SharedPtr<Mesh>> meshes;
        Utils::ProcessNode(scene->mRootNode, scene, meshes);

        return Model::Create(meshes);
    }
}