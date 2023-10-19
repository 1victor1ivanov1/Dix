#pragma once

#include "Dix/Core/Base.h"

namespace Dix
{
	enum class FramebufferTextureFormat
	{
		None = 0,

		//Color
		R8,
		RGB8,
		RGBA8,
		RGBA16F,
		RGBA32F,

		//Depth
		DEPTH24_STENCIL8,
	};

	struct FramebufferTextureSpecification
	{
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat textureFormat)
			: TextureFormat(textureFormat) {}

		FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
	};

	struct FramebufferAttachmentSpecification
	{
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
			: Attachments(attachments) {}

		std::vector<FramebufferTextureSpecification> Attachments;
	};

	struct FramebufferSpecification
	{
		u32 Width = 1, Height = 1;
		u32 Samples = 1;
		
		FramebufferAttachmentSpecification Attachments;
	};

	class Framebuffer
	{
	public:
		Framebuffer(const FramebufferSpecification& spec);
		~Framebuffer();

		void Invalidate();

		void BlitTo(SharedPtr<Framebuffer> other);

		const FramebufferSpecification& GetSpecification() const { return m_Specification; }
		const std::vector<FramebufferTextureSpecification>& GetColorAttachmentSpeifications() const { return m_ColorAttachmentSpecifications; }
		u32 GetRendererID() const { return m_RendererID; }
		u32 GetColorAttachmentID(u32 index) const { DIX_CORE_ASSERT(index < m_ColorAttachments.size());  return m_ColorAttachments[index]; }

		void Bind() const;
		void Unbind() const;

		static SharedPtr<Framebuffer> Create(const FramebufferSpecification& spec);

	private:
		FramebufferSpecification m_Specification;
		u32 m_RendererID = 0;

		std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
		std::vector<u32> m_ColorAttachments;

		FramebufferTextureSpecification m_DepthAttachmentSpecification = FramebufferTextureFormat::None;
		u32 m_DepthAttachment = 0;
	};
}