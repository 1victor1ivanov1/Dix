#pragma once

#include "Dix/Renderer/Texture.h"

namespace Dix
{
	class TextureImporter
	{
	public:
		TextureImporter() = delete;
		~TextureImporter() = delete;

		static SharedPtr<Texture2D> LoadTexture2D(const std::string& filepath, bool sRGB = false);
	};
}