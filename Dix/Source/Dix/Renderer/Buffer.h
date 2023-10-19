#pragma once

namespace Dix
{
	enum class ShaderDataType
	{
		None = 0,
		Int,
		Int2,
		Int3,
		Int4,
		Bool,
		Float,
		Float2,
		Float3,
		Float4,
		Mat3,
		Mat4,
	};

	static u32 ShaderTypeSize(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Int:		return 4;
			case ShaderDataType::Int2:		return 4 * 2;
			case ShaderDataType::Int3:		return 4 * 3;
			case ShaderDataType::Int4:		return 4 * 4;
			case ShaderDataType::Float:		return 4;
			case ShaderDataType::Float2:	return 4 * 2;
			case ShaderDataType::Float3:	return 4 * 3;
			case ShaderDataType::Float4:	return 4 * 4;
			case ShaderDataType::Bool:		return 1;
			case ShaderDataType::Mat3:		return 4 * 3 * 3;
			case ShaderDataType::Mat4:		return 4 * 4 * 4;
		}

		DIX_CORE_ASSERT(false, "Unknown shader data type!");
		return 0;
	}

	struct BufferElement
	{
		std::string Name;
		ShaderDataType Type;
		u32 Size;
		u32 Offset;
		bool Normalized;

		BufferElement() = default;

		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
			: Name(name), Type(type), Normalized(normalized), Offset(0), Size(ShaderTypeSize(type))
		{
		}

		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
				case ShaderDataType::Int:		return 1;
				case ShaderDataType::Int2:		return 2;
				case ShaderDataType::Int3:		return 3;
				case ShaderDataType::Int4:		return 4;
				case ShaderDataType::Float:		return 1;
				case ShaderDataType::Float2:	return 2;
				case ShaderDataType::Float3:	return 3;
				case ShaderDataType::Float4:	return 4;
				case ShaderDataType::Bool:		return 1;
				case ShaderDataType::Mat3:		return 3; // 3 * float3
				case ShaderDataType::Mat4:		return 4; // 4 * float4
			}

			DIX_CORE_ASSERT(false, "Unknown shader data type!");
			return 0;
		}
	};

	class BufferLayout
	{
	public:
		BufferLayout() {}

		BufferLayout(std::initializer_list<BufferElement> elements)
			: m_Elements(elements)
		{
			CalculateOffsetAndStride();
		}

		u32 GetStride() const { return m_Stride; }
		const std::vector<BufferElement>& GetElements() const { return m_Elements; }

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

	private:
		void CalculateOffsetAndStride()
		{
			for (auto& element : m_Elements)
			{
				element.Offset = m_Stride;
				m_Stride += element.Size;
			}
		}

	private:
		std::vector<BufferElement> m_Elements;
		u32 m_Stride = 0;
	};

	class VertexBuffer
	{
	public:
		VertexBuffer(u32 size);
		VertexBuffer(f32* vertices, u32 size);
		~VertexBuffer();

		void Bind() const;
		void Unbind() const;

		void SetData(const void* data, u32 size);

		const BufferLayout& GetLayout() const { return m_Layout; }
		void SetLayout(const BufferLayout& layout) { m_Layout = layout; }

		static SharedPtr<VertexBuffer> Create(u32 size);
		static SharedPtr<VertexBuffer> Create(f32* vertices, u32 size);

	private:
		BufferLayout m_Layout;
		u32 m_RendererID = 0;
	};

	class IndexBuffer
	{
	public:
		IndexBuffer(u32* indices, u32 size);
		~IndexBuffer();

		void Bind() const;
		void Unbind() const;

		u32 GetCount() const { return m_Count; }

		static SharedPtr<IndexBuffer> Create(u32* indices, u32 size);

	private:
		u32 m_RendererID = 0;
		u32 m_Count = 0;
	};
}