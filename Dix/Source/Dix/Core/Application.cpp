#include "dixpch.h"

#include "Dix/Core/Application.h"

#include "Dix/Renderer/Shader.h"
#include "Dix/Renderer/Texture.h"
#include "Dix/Renderer/VertexArray.h"
#include "Dix/Renderer/RenderCommand.h"

#include "Dix/Utils/PlatformUtils.h"

#include <glad/glad.h>

//for testing
#include "Dix/Renderer/Framebuffer.h"

namespace Dix
{
    Application* Application::s_Instance = nullptr;
    ApplicationTelemetry Application::s_Telemetry;

    Application::Application(const ApplicationSpecification& spec)
    {
        m_Specification = spec;

        Init();
    }

    Application::~Application()
    {
        Shutdown();
    }

    void Application::Run()
    {
        SharedPtr<Shader> shader = Shader::Create("C:/Users/1vvvi/Desktop/Dix/Sandbox/Assets/Shaders/test.glsl");
        SharedPtr<Shader> tonemappingShader = Shader::Create("C:/Users/1vvvi/Desktop/Dix/Sandbox/Assets/Shaders/Tonemapping.glsl");

        SharedPtr<Texture2D> texture = Texture2D::Create("C:/Users/1vvvi/Desktop/papka_for_papok/backgrounds/МояДевушка.jpg", true);

        FramebufferSpecification fSpec;
        fSpec.Width = 1280;
        fSpec.Height = 720;
        fSpec.Samples = 4;
        fSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::DEPTH24_STENCIL8 };
        SharedPtr<Framebuffer> framebuffer = Framebuffer::Create(fSpec);

        FramebufferSpecification resolveFSpec;
        resolveFSpec.Width = 1280;
        resolveFSpec.Height = 720;
        resolveFSpec.Samples = 1;
        resolveFSpec.Attachments = { FramebufferTextureFormat::RGBA8 };
        SharedPtr<Framebuffer> resolveFramebuffer = Framebuffer::Create(resolveFSpec);

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

        SharedPtr<VertexArray> vertexArray = VertexArray::Create();
        SharedPtr<VertexArray> tonemappingVertexArray = VertexArray::Create();

        vertexArray->AddVertexBuffer(vertexBuffer1);
        vertexArray->AddVertexBuffer(vertexBuffer2);
        vertexArray->SetIndexBuffer(indexBuffer);

        uint32_t emptyVao;
        glCreateVertexArrays(1, &emptyVao);

        while (m_Running)
        {
            float time = Time::GetTime();
            Timestep timestep = time - m_LastFrameTime;
            m_LastFrameTime = time;

            s_Telemetry.FrameTime = timestep;
            s_Telemetry.FPS = 1.0f / timestep;

            if (!m_Minimized)
            {
                framebuffer->Bind();

                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

                shader->Bind();
                texture->Bind();

                RenderCommand::DrawIndexed(vertexArray);

                shader->Unbind();

                framebuffer->Unbind();

                framebuffer->BlitTo(resolveFramebuffer, 0, 0);

                tonemappingShader->Bind();

                glBindTextureUnit(0, resolveFramebuffer->GetColorAttachmentID(0));
                glBindVertexArray(emptyVao);
                glDrawArrays(GL_TRIANGLES, 0, 3);

                tonemappingShader->Unbind();
            }

            m_Window->OnUpdate();
        }
    }

    void Application::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<WindowCloseEvent>(DIX_BIND_EVENT_CALLBACK(Application::OnWindowClose));
        dispatcher.Dispatch<WindowResizeEvent>(DIX_BIND_EVENT_CALLBACK(Application::OnWindowResize));
    }

    void Application::Init()
    {
        DIX_CORE_ASSERT(!s_Instance, "Instance of Application already exists!");
        s_Instance = this;

        m_Window = Window::Create();
        m_Window->SetEventCallback(DIX_BIND_EVENT_CALLBACK(Application::OnEvent));

        RenderCommand::Init();
    }

    void Application::Shutdown()
    {

    }

    bool Application::OnWindowClose(WindowCloseEvent& event)
    {
        m_Running = false;

        return false;
    }

    bool Application::OnWindowResize(WindowResizeEvent& event)
    {
        if (event.GetWidth() == 0 || event.GetHeight() == 0)
        {
            m_Minimized = true;
            return false;
        }

        m_Minimized = false;
        glViewport(0, 0, event.GetWidth(), event.GetHeight());

        return false;
    }
}