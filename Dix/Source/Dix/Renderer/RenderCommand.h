#pragma once

#include "Dix/Renderer/VertexArray.h"

namespace Dix
{
	class RenderCommand
	{
	public:
		RenderCommand() = delete;
		~RenderCommand() = delete;

		static void Init();
		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
		static void SetClearColor(const glm::vec4& color);
		static void Clear();

		static void DrawIndexed(const SharedPtr<VertexArray>& vertexArray, uint32_t indexCount = 0);
	private:
	};
}