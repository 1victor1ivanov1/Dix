#include "dixpch.h"

#include "Dix/Core/TestLayer.h"

#include "Dix/Renderer/RenderCommand.h"

#include "Dix/Asset/TextureImporter.h"
#include "Dix/Asset/ModelImporter.h"

#include "Dix/Utils/PlatformUtils.h"

#include <glad/glad.h>

namespace Dix
{
	void TestLayer::OnAttach() 
	{
		shader = Shader::Create("C:/Users/1vvvi/Desktop/Dix/Sandbox/Assets/Shaders/test.glsl");
		tonemappingShader = Shader::Create("C:/Users/1vvvi/Desktop/Dix/Sandbox/Assets/Shaders/Tonemapping.glsl");

		albedoTexture = TextureImporter::LoadTexture2D("C:/Users/1vvvi/Desktop/firelink_greatsword/textures/Material_baseColor.png", true);
		normalTexture = TextureImporter::LoadTexture2D("C:/Users/1vvvi/Desktop/firelink_greatsword/textures/Material_normal.png");
		metallicRoughnessTexture = TextureImporter::LoadTexture2D("C:/Users/1vvvi/Desktop/firelink_greatsword/textures/Material_metallicRoughness.png");;
		emissiveTexture = TextureImporter::LoadTexture2D("C:/Users/1vvvi/Desktop/firelink_greatsword/textures/Material_emissive.png");;
		model = ModelImporter::LoadModel("C:/Users/1vvvi/Desktop/firelink_greatsword/scene.gltf");

		FramebufferSpecification fSpec;
		fSpec.Width = 1280;
		fSpec.Height = 720;
		fSpec.Samples = 4;
		fSpec.Attachments = { FramebufferTextureFormat::RGBA16F, FramebufferTextureFormat::DEPTH24_STENCIL8 };
		framebuffer = Framebuffer::Create(fSpec);

		FramebufferSpecification resolveFSpec;
		resolveFSpec.Width = 1280;
		resolveFSpec.Height = 720;
		resolveFSpec.Samples = 1;
		resolveFSpec.Attachments = { FramebufferTextureFormat::RGBA16F };
		resolveFramebuffer = Framebuffer::Create(resolveFSpec);

		uniformBuffer = UniformBuffer::Create(sizeof(TransformData));

		tonemappingVertexArray = VertexArray::Create();

		camera = Camera(45.f, 1280.f / 720.f, 0.1f, 100.f);
	}

	void TestLayer::OnDetach()
	{

	}

	void TestLayer::OnUpdate(Timestep ts)
	{
		camera.OnUpdate(ts);

		glm::mat4 m(1.0f);

		transformData.ModelMatrix = m;
		transformData.ViewProjectionMatrix = camera.GetViewProjectionMatrix();
		uniformBuffer->SetData(&transformData, sizeof(TransformData));

		framebuffer->Bind();
		RenderCommand::SetClearColor({ 0.2f, 0.3f, 0.3f, 1.0f });
		RenderCommand::Clear();
		RenderCommand::EnableDepthTest();

		shader->Bind();
		albedoTexture->Bind(0);
		normalTexture->Bind(1);
		metallicRoughnessTexture->Bind(2);
		emissiveTexture->Bind(3);

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