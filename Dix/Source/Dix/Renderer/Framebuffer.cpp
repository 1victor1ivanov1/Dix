#include "dixpch.h"

#include "Dix/Renderer/Framebuffer.h"

#include <glad/glad.h>

namespace Dix
{
	static const u32 s_MaxFramebufferSize = 8192;

	namespace Utils
	{
		static bool IsDepthFormat(const FramebufferTextureFormat& format)
		{
			switch (format)
			{
				case FramebufferTextureFormat::DEPTH24_STENCIL8: return true;
			}

			return false;
		}

		static GLenum DixTextureFormatToOpenGLFormat(FramebufferTextureFormat format)
		{
			switch (format)
			{
				case FramebufferTextureFormat::R8:		return GL_R8;
				case FramebufferTextureFormat::RGB8:	return GL_RGB8;
				case FramebufferTextureFormat::RGBA8:	return GL_RGBA8;
				case FramebufferTextureFormat::RGBA16F: return GL_RGBA16F;
				case FramebufferTextureFormat::RGBA32F: return GL_RGBA32F;
			}

			DIX_CORE_ASSERT(false, "unknown framebuffer texture format!");
			return 0;
		}

		static u32 TextureTarget(bool multisampled)
		{
			return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}

		static void CreateTextures(bool multisampled, uint32_t* textures, uint32_t count)
		{
			glCreateTextures(TextureTarget(multisampled), count, textures);
		}

		static void BindTexture(bool multisampled, uint32_t texture)
		{
			glBindTexture(TextureTarget(multisampled), texture);
		}

		static void AttachColorTexture(uint32_t texture, i32 samples, GLenum format, i32 index, u32 width, u32 height)
		{
			bool multisampled = samples > 1;
			if (multisampled)
			{
				glTexStorage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
			}
			else
			{
				glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), texture, 0);
		}

		static void AttachDepthTexture(uint32_t texture, i32 samples, GLenum format, GLenum attachmentType, u32 width, u32 height)
		{
			bool multisampled = samples > 1;
			if (multisampled)
			{
				glTexStorage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
			}
			else
			{
				glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), texture, 0);
		}
	}

	Framebuffer::Framebuffer(const FramebufferSpecification& spec)
		: m_Specification(spec)
	{
		for (auto& spec : spec.Attachments.Attachments)
		{
			if (!Utils::IsDepthFormat(spec.TextureFormat))
				m_ColorAttachmentSpecifications.push_back(spec);
			else
				m_DepthAttachmentSpecification = spec;
		}

		Invalidate();
	}

	Framebuffer::~Framebuffer()
	{
        glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(m_ColorAttachments.size(), &m_ColorAttachments[0]);
		glDeleteTextures(1, &m_DepthAttachment);
	}

	void Framebuffer::Invalidate()
	{
		if (m_RendererID)
		{
			glDeleteFramebuffers(1, &m_RendererID);
			glDeleteTextures(m_ColorAttachments.size(), &m_ColorAttachments[0]);
			glDeleteTextures(1, &m_DepthAttachment);

			m_ColorAttachments.clear();
			m_DepthAttachment = 0;
		}

		glCreateFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		if (m_Specification.Width > s_MaxFramebufferSize)
		{
			DIX_CORE_WARN("Framebuffer width({0}) if bigger than max framebuffer width({1}): width change to max value!", m_Specification.Width, s_MaxFramebufferSize);
			m_Specification.Width = s_MaxFramebufferSize;
		}
		if (m_Specification.Height > s_MaxFramebufferSize)
		{
			DIX_CORE_WARN("Framebuffer height({0}) if bigger than max framebuffer height({1}): height change to max value!", m_Specification.Height, s_MaxFramebufferSize);
			m_Specification.Height = s_MaxFramebufferSize;
		}

		bool multisampled = m_Specification.Samples > 1;

		if (m_ColorAttachmentSpecifications.size() > 0)
		{
			m_ColorAttachments.resize(m_ColorAttachmentSpecifications.size());
			Utils::CreateTextures(multisampled, &m_ColorAttachments[0], m_ColorAttachments.size());

			for (i32 i = 0; i < m_ColorAttachments.size(); ++i)
			{
				Utils::BindTexture(multisampled, m_ColorAttachments[i]);
				Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, Utils::DixTextureFormatToOpenGLFormat(m_ColorAttachmentSpecifications[i].TextureFormat), i, m_Specification.Width, m_Specification.Height);
			}
		}

		if (m_DepthAttachmentSpecification.TextureFormat != FramebufferTextureFormat::None)
		{
			Utils::CreateTextures(multisampled, &m_DepthAttachment, 1);
			Utils::BindTexture(multisampled, m_DepthAttachment);
			switch (m_DepthAttachmentSpecification.TextureFormat)
			{
				case FramebufferTextureFormat::DEPTH24_STENCIL8:
					Utils::AttachDepthTexture(m_DepthAttachment, m_Specification.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Specification.Width, m_Specification.Height);
					break;
			}
		}

		if (m_ColorAttachments.size() > 1)
		{
			DIX_CORE_ASSERT(m_ColorAttachments.size() <= 4, "Framebuffer has more than 4 attachments!");
			GLenum buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(m_ColorAttachments.size(), buffers);
		}
		else if (m_ColorAttachments.empty())
		{
			glDrawBuffer(GL_NONE);
		}

		DIX_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}

	void Framebuffer::BlitTo(SharedPtr<Framebuffer> other)
	{
		u32 src_w = m_Specification.Width, src_h = m_Specification.Height;
		u32 dst_w = other->GetSpecification().Width, dst_h = other->GetSpecification().Height;

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, other->GetRendererID());
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_RendererID);
		glBlitFramebuffer(0, 0, src_w, src_h, 0, 0, dst_w, dst_h, GL_COLOR_BUFFER_BIT, GL_LINEAR);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Framebuffer::Resize(u32 width, u32 height)
	{
		if (width == 0 || height == 0)
		{
			DIX_CORE_WARN("Attempted to resize framebuffer to {0}, {1}!", width, height);
			return;
		}

		m_Specification.Width = width;
		m_Specification.Height = height;
		Invalidate();
	}

	void Framebuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_Specification.Width, m_Specification.Height);
	}

	void Framebuffer::Unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	SharedPtr<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		return MakeShared<Framebuffer>(spec);
	}
}