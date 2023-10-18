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
		uint32_t Width = 1, Height = 1;
		uint32_t Samples = 1;
		
		FramebufferAttachmentSpecification Attachments;
	};

	class Framebuffer
	{
	public:
		Framebuffer(const FramebufferSpecification& spec);
		~Framebuffer();

		void Invalidate();

		void BlitTo(SharedPtr<Framebuffer> other, uint32_t inID, uint32_t outID);

		const FramebufferSpecification& GetSpecification() const { return m_Specification; }
		const std::vector<FramebufferTextureSpecification>& GetColorAttachmentSpeifications() const { return m_ColorAttachmentSpecifications; }
		uint32_t GetRendererID() const { return m_RendererID; }
		uint32_t GetColorAttachmentID(uint32_t index) const { DIX_CORE_ASSERT(index < m_ColorAttachments.size());  return m_ColorAttachments[index]; }

		void Bind() const;
		void Unbind() const;

		static SharedPtr<Framebuffer> Create(const FramebufferSpecification& spec);

	private:
		FramebufferSpecification m_Specification;
		uint32_t m_RendererID = 0;

		std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
		std::vector<uint32_t> m_ColorAttachments;

		FramebufferTextureSpecification m_DepthAttachmentSpecification = FramebufferTextureFormat::None;
		uint32_t m_DepthAttachment = 0;
	};
}