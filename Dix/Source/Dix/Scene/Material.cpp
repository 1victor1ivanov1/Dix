#include "dixpch.h"

#include "Dix/Scene/Material.h"

namespace Dix
{
	Material::Material(SharedPtr<Shader>& shader)
		: m_Shader(shader)
	{
	}

	Material::~Material()
	{
	}

	void Material::Bind()
	{
		m_Shader->Bind();

		if (m_AlbedoTexture)
			m_AlbedoTexture->Bind(0);
		if (m_NormalTexture)
			m_AlbedoTexture->Bind(1);
		if (m_MetallicRoughnessTexture)
			m_AlbedoTexture->Bind(2);
	}

	void Material::Unbind()
	{
		m_Shader->Unbind();
	}
}