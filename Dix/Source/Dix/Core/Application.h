#pragma once

#include "Dix/Core/Window.h"
#include "Dix/Core/Timestep.h"
#include "Dix/Core/LayerStack.h"

#include "Dix/Events/Event.h"
#include "Dix/Events/ApplicationEvent.h"

#include "Dix/ImGui/ImGuiLayer.h"

namespace Dix
{
	struct ApplicationCommandLineArgs
	{
		i32 Size = 0;
		char** Args = nullptr;

		const char* operator[](i32 index)
		{
			DIX_CORE_ASSERT(index < Size);
			return Args[index];
		}
	};

	struct ApplicationSpecification
	{
		std::string Name = "Dix Application";
		std::string Workingirectory;
		ApplicationCommandLineArgs Args;

		ApplicationSpecification() = default;
	};

	class Application
	{
	public:
		Application(const ApplicationSpecification& spec);
		virtual ~Application();

		void Run();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		void OnEvent(Event& event);

		void* GetNativeWindow() const { return m_Window->GetNativeWindow(); }

		static Application* Get() { return s_Instance; }

	private:
		bool OnWindowClose(WindowCloseEvent& event);
		bool OnWindowResize(WindowResizeEvent& event);

	private:
		ApplicationSpecification m_Specification;

		UniquePtr<Window> m_Window;

		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;

		bool m_Running = true;
		bool m_Minimized = false;
		float m_LastFrameTime = 0.0f;

	private:
		static Application* s_Instance;
	};

	Application* CreateApplication(ApplicationCommandLineArgs args);
}