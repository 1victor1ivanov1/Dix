#include "dixpch.h"

#include "Dix/Core/Application.h"

#include "Dix/Renderer/RenderCommand.h"

#include "Dix/Utils/PlatformUtils.h"

#include <glad/glad.h>

#include <imgui.h>

//for testing
#include "Dix/Core/TestLayer.h"

namespace Dix
{
    Application* Application::s_Instance = nullptr;

    Application::Application(const ApplicationSpecification& spec)
        : m_Specification(spec)
    {
        DIX_CORE_ASSERT(!s_Instance, "Instance of Application already exists!");
        s_Instance = this;

        m_Window = Window::Create(WindowProperties(m_Specification.Name));
        m_Window->SetEventCallback(DIX_BIND_EVENT_CALLBACK(Application::OnEvent));

        RenderCommand::Init();

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);

        Layer* testLayer = new TestLayer();
        PushLayer(testLayer);
    }

    Application::~Application()
    {
    }

    void Application::Run()
    {

        while (m_Running)
        {
            f32 time = Time::GetTime();
            Timestep timestep = time - m_LastFrameTime;
            m_LastFrameTime = time;

            if (!m_Minimized)
            {
                for (auto layer : m_LayerStack)
                {
                    layer->OnUpdate(timestep);
                }

                m_ImGuiLayer->Begin();
                for (auto layer : m_LayerStack)
                {
                    layer->OnImGuiRender();
                }

                ImGui::Begin("Perfomance");
                ImGui::Text("Frame time: %.3f ms", timestep.GetMilliseconds());
                ImGui::Text("FPS: %.1f", 1 / timestep);
                ImGui::End();

                m_ImGuiLayer->End();
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