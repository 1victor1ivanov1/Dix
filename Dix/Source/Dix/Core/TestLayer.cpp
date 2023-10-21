#include "dixpch.h"

#include "Dix/Core/TestLayer.h"

#include "Dix/Renderer/RenderCommand.h"

#include "Dix/Utils/PlatformUtils.h"

#include <glad/glad.h>

namespace Dix
{
	void TestLayer::OnAttach() 
	{
		shader = Shader::Create("C:/Users/1vvvi/Desktop/Dix/Sandbox/Assets/Shaders/test.glsl");
		tonemappingShader = Shader::Create("C:/Users/1vvvi/Desktop/Dix/Sandbox/Assets/Shaders/Tonemapping.glsl");

		texture = Texture2D::Create("C:/Users/1vvvi/Desktop/viking_room/viking_room.png", true);

		model = Model::Create("C:/Users/1vvvi/Desktop/viking_room/viking_room.obj");

		FramebufferSpecification fSpec;
		fSpec.Width = 1280;
		fSpec.Height = 720;
		fSpec.Samples = 4;
		fSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::DEPTH24_STENCIL8 };
		framebuffer = Framebuffer::Create(fSpec);

		FramebufferSpecification resolveFSpec;
		resolveFSpec.Width = 1280;
		resolveFSpec.Height = 720;
		resolveFSpec.Samples = 1;
		resolveFSpec.Attachments = { FramebufferTextureFormat::RGBA8 };
		resolveFramebuffer = Framebuffer::Create(resolveFSpec);

		tonemappingVertexArray = VertexArray::Create();

		camera = Camera(45.f, 1280.f / 720.f, 0.1f, 100.f);
	}

	void TestLayer::OnDetach()
	{

	}

	void TestLayer::OnUpdate(Timestep ts)
	{
		camera.OnUpdate(ts);

		framebuffer->Bind();
		RenderCommand::SetClearColor({ 0.2f, 0.3f, 0.3f, 1.0f });
		RenderCommand::Clear();
		RenderCommand::EnableDepthTest();

		shader->Bind();
		texture->Bind();

		glm::mat4 m(1.0f);
		m = glm::rotate(m, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		m = glm::rotate(m, glm::radians(-90.0f), glm::vec3(0, 0, 1));

		shader->SetMat("u_ModelViewProjectionMatrix", camera.GetViewProjectionMatrix() * m);
		model->Render();
		
		shader->Unbind();
		framebuffer->Unbind();

		framebuffer->BlitTo(resolveFramebuffer);

		tonemappingShader->Bind();
		glBindTextureUnit(0, resolveFramebuffer->GetColorAttachmentID(0));
		RenderCommand::DrawArrays(tonemappingVertexArray, 3);
		tonemappingShader->Unbind();
	}

	void TestLayer::OnImGuiRender()
	{
		camera.OnImGuiRender();
	}

	void TestLayer::OnEvent(Event& event)
	{
		camera.OnEvent(event);

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowResizeEvent>(DIX_BIND_EVENT_CALLBACK(TestLayer::OnWindowResize));
	}

	bool TestLayer::OnWindowResize(WindowResizeEvent& event)
	{
		framebuffer->Resize(event.GetWidth(), event.GetHeight());
		resolveFramebuffer->Resize(event.GetWidth(), event.GetHeight());

		return false;
	}
}