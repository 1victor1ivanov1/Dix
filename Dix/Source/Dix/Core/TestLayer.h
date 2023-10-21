#pragma once

#include "Dix/Core/Layer.h"
#include "Dix/Renderer/Buffer.h"
#include "Dix/Renderer/VertexArray.h"
#include "Dix/Renderer/Framebuffer.h"
#include "Dix/Renderer/Texture.h"
#include "Dix/Renderer/Shader.h"
#include "Dix/Renderer/Camera.h"

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

		SharedPtr<Model> model;
		SharedPtr<Texture2D> texture;

		Camera camera;

		glm::vec3 cubePositions[10] = {
			glm::vec3(0.0f,  0.0f,  0.0f),
			glm::vec3(2.0f,  5.0f, -15.0f),
			glm::vec3(-1.5f, -2.2f, -2.5f),
			glm::vec3(-3.8f, -2.0f, -12.3f),
			glm::vec3(2.4f, -0.4f, -3.5f),
			glm::vec3(-1.7f,  3.0f, -7.5f),
			glm::vec3(1.3f, -2.0f, -2.5f),
			glm::vec3(1.5f,  2.0f, -2.5f),
			glm::vec3(1.5f,  0.2f, -1.5f),
			glm::vec3(-1.3f,  1.0f, -1.5f)
		};
	};
}