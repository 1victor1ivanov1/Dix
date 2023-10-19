#pragma once

#include "Dix/Core/Base.h"

struct GLFWwindow;

namespace Dix
{
	class GraphicsContext;
	class Event;

	struct WindowProperties
	{
		u32 Width, Height;
		std::string Name;

		WindowProperties(const std::string& name = "Dix", u32 width = 1280, u32 height = 720) : Name(name), Width(width), Height(height) {}
	};

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

	public:
		Window(const WindowProperties& props);
		~Window();

		void OnUpdate();

		u32 GetWidth() const { return m_Data.Width; }
		u32 GetHeight() const { return m_Data.Height; }

		void* GetNativeWindow();

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
			u32 Width, Height;
			std::string Name;
			bool VSync;

			EventCallbackFn Callback;
		} m_Data;
	};
}