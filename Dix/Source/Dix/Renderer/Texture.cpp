#include "dixpch.h"

#include "Dix/Renderer/Texture.h"

#include <glad/glad.h>
#include <stb_image.h>

namespace Dix
{
	Texture2D::Texture2D(const TextureSpecification& spec, const Buffer& buffer, bool sRGB)
		: m_Specification(spec)
	{
		GLenum internalFormat = 0, dataFormat = 0;

		switch (spec.Format)
		{
			case ImageFormat::RGBA8:
				internalFormat = sRGB ? GL_SRGB8_ALPHA8 : GL_RGBA8;
				dataFormat = GL_RGBA;
				break;
			case ImageFormat::RGB8:
				internalFormat = sRGB ? GL_SRGB8 : GL_RGB8;
				dataFormat = GL_RGB;
				break;
			case ImageFormat::RG8:
				internalFormat = GL_RG8;
				dataFormat = GL_RG;
				break;
			case ImageFormat::R8:
				internalFormat = GL_R8;
				dataFormat = GL_RED;
				break;
		}

		DIX_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureStorage2D(m_RendererID, 4, internalFormat, spec.Width, spec.Height);
		glTextureSubImage2D(m_RendererID, 0, 0, 0, spec.Width, spec.Height, dataFormat, GL_UNSIGNED_BYTE, buffer.Data);
		glGenerateTextureMipmap(m_RendererID);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	Texture2D::~Texture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void Texture2D::Bind(u32 slot) const
	{
		DIX_CORE_ASSERT(slot < 16, "Invalid slot number!");
		glBindTextureUnit(slot, m_RendererID);
	}

	SharedPtr<Texture2D> Texture2D::Create(const TextureSpecification& spec, const Buffer& buffer, bool sRGB)
	{
		return MakeShared<Texture2D>(spec, buffer, sRGB);
	}
}