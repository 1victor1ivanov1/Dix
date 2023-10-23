#include "dixpch.h"

#include "Dix/Renderer/RenderCommand.h"

#include "Dix/Scene/Mesh.h"

namespace Dix
{
	Mesh::Mesh(std::vector<u32>& indicies, std::vector<MeshVertex>& vertexes)
	{
		auto indexBuffer = IndexBuffer::Create(indicies.data(), indicies.size());

		auto vertexBuffer = VertexBuffer::Create(&vertexes[0].Position[0], vertexes.size() * sizeof(MeshVertex));
		vertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Float3, "a_Normal" },
			{ ShaderDataType::Float3, "a_Tangent" },
			{ ShaderDataType::Float3, "a_Bitangent" }
		});

		m_VertexArray = VertexArray::Create();
		m_VertexArray->AddVertexBuffer(vertexBuffer);
		m_VertexArray->SetIndexBuffer(indexBuffer);
	}

	Mesh::~Mesh()
	{
	}

	void Mesh::Render() const
	{
		RenderCommand::DrawIndexed(m_VertexArray);
	}

	SharedPtr<Mesh> Mesh::Create(std::vector<u32>& indicies, std::vector<MeshVertex>& vertexes)
	{
		return MakeShared<Mesh>(indicies, vertexes);
	}
}