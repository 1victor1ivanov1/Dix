#pragma once

#include "Dix/Core/Layer.h"
#include "Dix/Renderer/Buffer.h"
#include "Dix/Renderer/VertexArray.h"
#include "Dix/Renderer/Framebuffer.h"
#include "Dix/Renderer/Texture.h"
#include "Dix/Renderer/Shader.h"

namespace Dix
{
	class TestLayer : public Layer
	{
	public:
		TestLayer() : Layer("TestLayer") {}
		virtual ~TestLayer() {}

		virtual void OnAttach() override;
		
		virtual void OnDetach() override;

		virtual void OnUpdate(Timestep ts) override;
	
		virtual void OnEvent(Event& event) override;

	private:
		SharedPtr<Shader> shader;
		SharedPtr<Shader> tonemappingShader;

		SharedPtr<Texture2D> texture;

		SharedPtr<Framebuffer> framebuffer;
		SharedPtr<Framebuffer> resolveFramebuffer;

		SharedPtr<VertexArray> vertexArray;
		SharedPtr<VertexArray> tonemappingVertexArray;
	};
}