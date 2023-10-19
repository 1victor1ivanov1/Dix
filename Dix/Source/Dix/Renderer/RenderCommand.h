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
		static void SetViewport(u32 x, u32 y, u32 width, u32 height);
		static void SetClearColor(const glm::vec4& color);
		static void Clear();

		static void EnableDepthTest();
		static void DisableDepthTest();

		static void DrawIndexed(const SharedPtr<VertexArray>& vertexArray, u32 indexCount = 0);
		static void DrawArrays(const SharedPtr<VertexArray>& vertexArray, u32 vertexCount = 0);
	private:
	};
}