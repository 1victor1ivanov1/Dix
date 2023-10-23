#pragma once

#include "Dix/Core/Layer.h"
#include "Dix/Renderer/Buffer.h"
#include "Dix/Renderer/VertexArray.h"
#include "Dix/Renderer/Framebuffer.h"
#include "Dix/Renderer/Texture.h"
#include "Dix/Renderer/Shader.h"
#include "Dix/Renderer/Camera.h"
#include "Dix/Renderer/UniformBuffer.h"

#include "Dix/Events/ApplicationEvent.h"

#include "Dix/Scene/Model.h"

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

		virtual void OnImGuiRender() override;
	
		virtual void OnEvent(Event& event) override;

	private:
		bool OnWindowResize(WindowResizeEvent& event);

	private:
		SharedPtr<Shader> shader;
		SharedPtr<Shader> tonemappingShader;

		SharedPtr<Framebuffer> framebuffer;
		SharedPtr<Framebuffer> resolveFramebuffer;

		SharedPtr<VertexArray> tonemappingVertexArray;

		SharedPtr<UniformBuffer> uniformBuffer;

		SharedPtr<Model> model;
		SharedPtr<Texture2D> albedoTexture;
		SharedPtr<Texture2D> normalTexture;
		SharedPtr<Texture2D> metallicRoughnessTexture;
		SharedPtr<Texture2D> emissiveTexture;

		struct TransformData
		{
			glm::mat4 ModelMatrix;
			glm::mat4 ViewProjectionMatrix;
		} transformData;

		Camera camera;
	};
}