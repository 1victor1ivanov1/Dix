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
		u32 Width = 1, Height = 1;
		bool GenerateMipmaps = false;
		u32 MipmapLevels = 0;
		ImageFormat Format = ImageFormat::None;

	};

	class Texture2D
	{
	public:
		Texture2D(const std::string& filepath, bool sRGB);
		~Texture2D();

		const TextureSpecification& GetSpecification() const { return m_Specification; }

		u32 GetWidth() const { return m_Width; }
		u32 GetHeight() const { return m_Height; }

		const std::string& GetPath() const { return m_Path; }

		void Bind(u32 slot = 0) const;

		static SharedPtr<Texture2D> Create(const std::string& filepath, bool sRGB = false);

	private:
		u32 m_RendererID = 0;

		TextureSpecification m_Specification;
		u32 m_Width = 0, m_Height = 0;
		u32 m_InternalFormat = 0, m_DataFormat = 0;

		std::string m_Path;
	};
}