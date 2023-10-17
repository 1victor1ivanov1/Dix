#pragma once

#include "Dix/Core/Base.h"

struct GLFWwindow;

namespace Dix
{
	class GraphicsContext;
	class Event;

	struct WindowProperties
	{
		uint32_t Width, Height;
		std::string Name;

		WindowProperties(const std::string& name = "Dix", uint32_t width = 1280, uint32_t height = 720) : Name(name), Width(width), Height(height) {}
	};

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

	public:
		Window(const WindowProperties& props);
		~Window();

		void OnUpdate();

		uint32_t GetWidth() const { return m_Data.Width; }
		uint32_t GetHeight() const { return m_Data.Height; }

		void SetEventCallback(const EventCallbackFn& callback);
		void SetVSync(bool enable);
		bool IsVSync();

		void* GetNativeWindow() const { return m_Window; }

		static UniquePtr<Window> Create(const WindowProperties& props = WindowProperties()) { return MakeUnique<Window>(props); }

	private:
		void Init();
		void Shutdown();

	private:
		GLFWwindow* m_Window;
		UniquePtr<GraphicsContext> m_Context;

		struct WindowData
		{
			uint32_t Width, Height;
			std::string Name;
			bool VSync;

			EventCallbackFn Callback;
		} m_Data;
	};
}