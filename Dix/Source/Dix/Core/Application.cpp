#include "dixpch.h"

#include "Dix/Core/Application.h"

#include "Dix/Renderer/Shader.h"
#include "Dix/Renderer/Texture.h"
#include "Dix/Renderer/VertexArray.h"
#include "Dix/Renderer/Framebuffer.h"
#include "Dix/Renderer/RenderCommand.h"

#include "Dix/Utils/PlatformUtils.h"

#include <glad/glad.h>

//for testing
#include "Dix/Core/TestLayer.h"

namespace Dix
{
    Application* Application::s_Instance = nullptr;
    ApplicationTelemetry Application::s_Telemetry;

    Application::Application(const ApplicationSpecification& spec)
    {
        m_Specification = spec;

        Init();

        Layer* testLayer = new TestLayer();
        PushLayer(testLayer);
    }

    Application::~Application()
    {
        Shutdown();
    }

    void Application::Run()
    {

        while (m_Running)
        {
            float time = Time::GetTime();
            Timestep timestep = time - m_LastFrameTime;
            m_LastFrameTime = time;

            s_Telemetry.FrameTime = timestep;
            s_Telemetry.FPS = 1.0f / timestep;

            if (!m_Minimized)
            {
                for (auto layer : m_LayerStack)
                {
                    layer->OnUpdate(timestep);
                }

                for (auto layer : m_LayerStack)
                {
                    layer->OnImGuiRender();
                }
            }

            m_Window->OnUpdate();
        }
    }

    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* overlay)
    {
        m_LayerStack.PushOverlay(overlay);
        overlay->OnAttach();
    }

    void Application::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<WindowCloseEvent>(DIX_BIND_EVENT_CALLBACK(Application::OnWindowClose));
        dispatcher.Dispatch<WindowResizeEvent>(DIX_BIND_EVENT_CALLBACK(Application::OnWindowResize));

        for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
        {
            if (event.Handled)
                break;
            (*it)->OnEvent(event);
        }
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

        return true;
    }

    bool Application::OnWindowResize(WindowResizeEvent& event)
    {
        if (event.GetWidth() == 0 || event.GetHeight() == 0)
        {
            m_Minimized = true;
            return false;
        }

        m_Minimized = false;
        RenderCommand::SetViewport(0, 0, event.GetWidth(), event.GetHeight());

        return false;
    }
}