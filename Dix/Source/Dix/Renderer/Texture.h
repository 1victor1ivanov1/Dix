#pragma once

#include "Dix/Core/Buffer.h"

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
		Texture2D(const TextureSpecification& spec, const Buffer& buffer, bool sRGB);
		~Texture2D();

		const TextureSpecification& GetSpecification() const { return m_Specification; }

		u32 GetWidth() const { return m_Specification.Width; }
		u32 GetHeight() const { return m_Specification.Height; }

		void Bind(u32 slot = 0) const;

		static SharedPtr<Texture2D> Create(const TextureSpecification& spec, const Buffer& buffer = Buffer(), bool sRGB = false);

	private:
		u32 m_RendererID = 0;

		TextureSpecification m_Specification;
	};
}