#pragma once

#include "Dix/Core/Base.h"

#include "Dix/Scene/Model.h"

namespace Dix
{
	class ModelImporter
	{
	public:
		ModelImporter() = delete;
		~ModelImporter() = delete;

		static SharedPtr<Model> LoadObjModel(const std::filesystem::path& filepath);
		static SharedPtr<Model> LoadGLTFModel(const std::filesystem::path& filepath);
	};
}