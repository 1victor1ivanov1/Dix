#include "dixpch.h"

#include "Dix/Core/Window.h"

#include "Dix/Renderer/GraphicsContext.h"

#include "Dix/Events/Event.h"
#include "Dix/Events/ApplicationEvent.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Dix
{
	Window::Window(const WindowProperties& props)
	{
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;
		m_Data.Name = props.Name;

		Init();
	}

	Window::~Window()
	{
		Shutdown();
	}

	void Window::OnUpdate()
	{
		m_Context->SwapBuffers();
		glfwPollEvents();
	}

	void Window::SetEventCallback(const EventCallbackFn& callback)
	{
		m_Data.Callback = callback;
	}

	void Window::SetVSync(bool enable)
	{
		if (enable)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);
		m_Data.VSync = enable;
	}

	bool Window::IsVSync()
	{
		return m_Data.VSync;
	}

	void Window::Init()
	{
		int success = glfwInit();
		DIX_CORE_ASSERT(success, "Failed to init GLFW!");

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef DIX_DEBUG
			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

		DIX_CORE_INFO("Creating a window ({0} ({1} {2}))", m_Data.Name, m_Data.Width, m_Data.Height);
		m_Window = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.Name.c_str(), nullptr, nullptr);
		DIX_CORE_ASSERT(m_Window, "Failed to create window!");

		m_Context = GraphicsContext::Create(m_Window);

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		glfwSetErrorCallback([](int error, const char* discription) 
		{
			DIX_CORE_ERROR("GLFW Error occured ({0}): {1}", error, discription);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data->Callback(event);
		});

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
			WindowResizeEvent event(width, height);
			data->Callback(event);
		});
	}

	void Window::Shutdown()
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}
}