#pragma once

#include "Dix/Core/Base.h"

namespace Dix
{
	enum class ImageFormat
	{
		None = 0,

		R8,
		RG8,
		RGB8,
		RGBA8,
	};

	struct TextureSpecification
	{
		uint32_t Width = 1, Height = 1;
		bool Mipmap = false;
		uint32_t MipmapLevels = 0;
		ImageFormat Format = ImageFormat::None;
	};

	class Texture2D
	{
	public:
		Texture2D(const std::string& filepath, bool sRGB);
		~Texture2D();

		const TextureSpecification& GetSpecification() const { return m_Specification; }

		uint32_t GetWidth() const { return m_Specification.Width; }
		uint32_t GetHeight() const { return m_Specification.Height; }

		const std::string& GetPath() const { return m_Path; }

		void Bind(uint32_t slot = 0) const;

		static SharedPtr<Texture2D> Create(const std::string& filepath, bool sRGB = false);

	private:
		uint32_t m_RendererID = 0;

		TextureSpecification m_Specification;
		uint32_t m_InternalFormat = 0, m_DataFormat = 0;

		std::string m_Path;
	};
}