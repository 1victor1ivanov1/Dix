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

		texture = Texture2D::Create("C:/Users/1vvvi/Desktop/papka_for_papok/backgrounds/крутой куджицу.png", true);

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

		float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
		};

		SharedPtr<VertexBuffer> vertexBuffer;
		SharedPtr<IndexBuffer> indexBuffer;

		vertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));
		vertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoords" },
		});

		vertexArray = VertexArray::Create();
		tonemappingVertexArray = VertexArray::Create();

		vertexArray->AddVertexBuffer(vertexBuffer);
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

		for (i32 i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			
			shader->SetMat("u_ModelViewProjectionMatrix", camera.GetViewProjectionMatrix() * model);

			RenderCommand::DrawArrays(vertexArray, 36);
		}
		
		shader->Unbind();
		framebuffer->Unbind();

		framebuffer->BlitTo(resolveFramebuffer);

		tonemappingShader->Bind();
		glBindTextureUnit(0, resolveFramebuffer->GetColorAttachmentID(0));
		RenderCommand::DrawArrays(tonemappingVertexArray, 3);
		tonemappingShader->Unbind();
	}

	void TestLayer::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowResizeEvent>(DIX_BIND_EVENT_CALLBACK(TestLayer::OnWindowResize));

		camera.OnEvent(event);
	}

	bool TestLayer::OnWindowResize(WindowResizeEvent& event)
	{
		framebuffer->Resize(event.GetWidth(), event.GetHeight());
		resolveFramebuffer->Resize(event.GetWidth(), event.GetHeight());

		return false;
	}
}