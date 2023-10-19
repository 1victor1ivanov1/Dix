#include "dixpch.h"

#include "Dix/Renderer/Buffer.h"

#include <glad/glad.h>

namespace Dix
{
	// Vertex Buffer
	VertexBuffer::VertexBuffer(u32 size)
	{
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	VertexBuffer::VertexBuffer(f32* vertices, u32 size)
	{
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW);
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void VertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	void VertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexBuffer::SetData(const void* data, u32 size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	SharedPtr<VertexBuffer> VertexBuffer::Create(u32 size)
	{
		return MakeShared<VertexBuffer>(size);
	}

	SharedPtr<VertexBuffer> VertexBuffer::Create(f32* vertices, u32 size)
	{
		return MakeShared<VertexBuffer>(vertices, size);
	}

	// Index Buffer
	IndexBuffer::IndexBuffer(u32* indices, u32 size)
		: m_Count(size)
	{
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size * sizeof(u32), indices, GL_STATIC_DRAW);
	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void IndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}

	void IndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	SharedPtr<IndexBuffer> IndexBuffer::Create(u32* indices, u32 size)
	{
		return MakeShared<IndexBuffer>(indices, size);
	}
}