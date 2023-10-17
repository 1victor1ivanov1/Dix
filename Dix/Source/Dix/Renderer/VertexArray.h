#pragma once

#include "Dix/Core/Base.h"

#include "Dix/Renderer/Buffer.h"

namespace Dix
{
	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		void Bind() const;
		void Unbind() const;

		void AddVertexBuffer(const SharedPtr<VertexBuffer> vertexBuffer);
		void SetIndexBuffer(const SharedPtr<IndexBuffer> indexBuffer);

		const std::vector<SharedPtr<VertexBuffer>> GetVertexBuffers() const { return m_VertexBuffers; }
		const SharedPtr<IndexBuffer> GetIndexBuffer() const { return m_IndexBuffer; }

		static SharedPtr<VertexArray> Create();

	private:
		uint32_t m_RendererID = 0;
		uint32_t m_VertexBufferIndex = 0;
		std::vector<SharedPtr<VertexBuffer>> m_VertexBuffers;
		SharedPtr<IndexBuffer> m_IndexBuffer;
	};
}