#pragma once

#include "Dix/Core/Base.h"

#include "Dix/Renderer/Buffer.h"
#include "Dix/Renderer/VertexArray.h"

#include <functional>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

namespace Dix
{
	struct MeshVertex
	{
		glm::vec3 Position;
		glm::vec2 TexCoord;
		glm::vec3 Normal;
		//glm::vec3 Tangent;
		//glm::vec3 Bitangent;

		bool operator==(const MeshVertex& other) const {
			return Position == other.Position && TexCoord == other.TexCoord && Normal == other.Normal/* && Tangent == other.Tangent && Bitangent == other.Bitangent*/;
		}
	};

	class Mesh
	{
	public:
		Mesh(std::vector<u32>& indicies, std::vector<MeshVertex>& vertexes);
		~Mesh();

		void Render() const;

		static SharedPtr<Mesh> Create(std::vector<u32>& indicies, std::vector< MeshVertex>& vertexes);

	private:
		SharedPtr<VertexArray>	m_VertexArray;
	};
}

namespace std
{
	template<>
	struct hash<Dix::MeshVertex>
	{
		u64 operator()(Dix::MeshVertex const& vertex) const {
			return ((hash<glm::vec3>{}(vertex.Position) ^
				(hash<glm::vec2>{}(vertex.TexCoord) << 1)) >> 1) ^
				(hash<glm::vec3>{}(vertex.Normal) << 1);
		}
	};
}