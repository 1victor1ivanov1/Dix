#include "dixpch.h"

#include "Dix/Renderer/Texture.h"

#include <glad/glad.h>
#include <stb_image.h>

namespace Dix
{
	Texture2D::Texture2D(const std::string& filepath, bool sRGB)
		: m_Path(filepath)
	{
		stbi_set_flip_vertically_on_load(true);

		int32_t width, height, channels;
		stbi_uc* data = nullptr;
		data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);

		if (data)
		{
			m_Specification.Width = width;
			m_Specification.Height = height;

			GLenum internalFormat = 0, dataFormat = 0;

			if (channels == 4)
			{
				internalFormat = sRGB ? GL_SRGB8_ALPHA8 : GL_RGBA8;
				dataFormat = GL_RGBA;
			}
			else if (channels == 3)
			{
				internalFormat = sRGB ? GL_SRGB8 : GL_RGB8;
				dataFormat = GL_RGB;
			}
			else if (channels == 2)
			{
				internalFormat = GL_RG8;
				dataFormat = GL_RG;
			}
			else if (channels == 1)
			{
				internalFormat = GL_R8;
				dataFormat = GL_RED;
			}

			DIX_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

			glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);

			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTextureStorage2D(m_RendererID, 4, internalFormat, width, height);
			glTextureSubImage2D(m_RendererID, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, data);
			glGenerateTextureMipmap(m_RendererID);

			glBindTexture(GL_TEXTURE_2D, 0);
		}

		stbi_image_free(data);
	}

	Texture2D::~Texture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void Texture2D::Bind(uint32_t slot) const
	{
		DIX_CORE_ASSERT(slot < 16, "Invalid slot number!");
		glBindTextureUnit(slot, m_RendererID);
	}

	SharedPtr<Texture2D> Texture2D::Create(const std::string& filepath, bool sRGB)
	{
		return MakeShared<Texture2D>(filepath, sRGB);
	}
}