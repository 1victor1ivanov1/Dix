#include "dixpch.h"

#include "Dix/Core/TestLayer.h"

#include "Dix/Renderer/RenderCommand.h"

#include <glad/glad.h>

namespace Dix
{
	void TestLayer::OnAttach() 
	{
		shader = Shader::Create("C:/Users/1vvvi/Desktop/Dix/Sandbox/Assets/Shaders/test.glsl");
		tonemappingShader = Shader::Create("C:/Users/1vvvi/Desktop/Dix/Sandbox/Assets/Shaders/Tonemapping.glsl");

		texture = Texture2D::Create("C:/Users/1vvvi/Desktop/papka_for_papok/backgrounds/МояДевушка.jpg", true);

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
			// positions          // colors           // texture coords
			 0.5f,  0.5f, 0.0f,       // top right
			 0.5f, -0.5f, 0.0f,       // bottom right
			-0.5f, -0.5f, 0.0f,      // bottom left
			-0.5f,  0.5f, 0.0f,        // top left 
		};
		float texCoords[] = {
			 1.0f, 1.0f,
			 1.0f, 0.0f,
			 0.0f, 0.0f,
			 0.0f, 1.0f
		};
		float vertices2[] = {
			1.0f, 3.0f,
			-3.0f, -1.0f,
			1.0f, -1.0f,
		};
		unsigned int indices[] = {
			0, 1, 3, // first triangle
			1, 2, 3  // second triangle
		};

		SharedPtr<VertexBuffer> vertexBuffer1;
		SharedPtr<VertexBuffer> vertexBuffer2;
		SharedPtr<IndexBuffer> indexBuffer;

		vertexBuffer1 = VertexBuffer::Create(vertices, sizeof(vertices));
		vertexBuffer1->SetLayout({
			{ ShaderDataType::Float3, "a_Position" }
			});
		vertexBuffer2 = VertexBuffer::Create(texCoords, sizeof(texCoords));
		vertexBuffer2->SetLayout({
			{ ShaderDataType::Float2, "a_TexCoords" }
			});
		indexBuffer = IndexBuffer::Create(indices, 6);

		vertexArray = VertexArray::Create();
		tonemappingVertexArray = VertexArray::Create();

		vertexArray->AddVertexBuffer(vertexBuffer1);
		vertexArray->AddVertexBuffer(vertexBuffer2);
		vertexArray->SetIndexBuffer(indexBuffer);
	}

	void TestLayer::OnDetach()
	{

	}

	void TestLayer::OnUpdate(Timestep ts)
	{
		framebuffer->Bind();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		shader->Bind();
		texture->Bind();

		RenderCommand::DrawIndexed(vertexArray);

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

	}
}