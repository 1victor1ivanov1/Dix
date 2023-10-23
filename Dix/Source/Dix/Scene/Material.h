#pragma once

#include "Dix/Renderer/Shader.h"
#include "Dix/Renderer/Texture.h"

namespace Dix
{
	class Material
	{
	public:
		Material() = default;
		Material(SharedPtr<Shader>& shader);
		~Material();

		void Bind();
		void Unbind();

	private:
		SharedPtr<Shader> m_Shader;

		SharedPtr<Texture2D> m_AlbedoTexture;
		SharedPtr<Texture2D> m_NormalTexture;
		SharedPtr<Texture2D> m_MetallicRoughnessTexture;
	};
}