#include "dixpch.h"
#include "Dix/Renderer/GraphicsContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Dix
{
	GraphicsContext::GraphicsContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		DIX_CORE_ASSERT(windowHandle, "Window handle cannot be null!");

		Init();
	}

	void GraphicsContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

	void GraphicsContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		i32 success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		DIX_CORE_ASSERT(success, "Failed to init OpengGL!");

		DIX_CORE_INFO("OpenGL Info:");
		const char* vendor = (char*)glGetString(GL_VENDOR);
		DIX_CORE_INFO("  Vendor: {0}", vendor);
		const char* renderer = (char*)glGetString(GL_RENDERER);
		DIX_CORE_INFO("  Renderer: {0}", renderer);
		const char* version = (char*)glGetString(GL_VERSION);
		DIX_CORE_INFO("  Version: {0}", version);

		DIX_CORE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 6), "Dix requires at least OpenGL version 4.6!");
	}
}