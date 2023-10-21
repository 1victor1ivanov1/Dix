#include "dixpch.h"

#include "Dix/Asset/TextureImporter.h"

#include <stb_image.h>

namespace Dix
{
	SharedPtr<Texture2D> TextureImporter::LoadTexture2D(const std::filesystem::path& filepath, bool sRGB)
	{
		stbi_set_flip_vertically_on_load(true);

		i32 width, height, channels;
		Buffer buffer;

		std::string pathString = filepath.string();
		buffer.Data = stbi_load(pathString.c_str(), &width, &height, &channels, 0);
		buffer.Size = width * height * channels;

		if (!buffer.Data)
		{
			DIX_CORE_ERROR("TextureImporter::LoadTexture2D - Failed to load texture at path {0}", filepath);
			return nullptr;
		}

		TextureSpecification spec;
		spec.Width = width;
		spec.Height = height;

		switch (channels)
		{
			case 4:
				spec.Format = ImageFormat::RGBA8;
				break;
			case 3:
				spec.Format = ImageFormat::RGB8;
				break;
			case 2:
				spec.Format = ImageFormat::RG8;
				break;
			case 1:
				spec.Format = ImageFormat::R8;
				break;
		}

		return Texture2D::Create(spec, buffer, sRGB);
	}
}