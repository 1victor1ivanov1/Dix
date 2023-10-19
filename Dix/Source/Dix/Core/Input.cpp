#include "dixpch.h"

#include "Dix/Core/Input.h"
#include "Dix/Core/Application.h"

#include <GLFW/glfw3.h>

namespace Dix
{
	bool Input::IsKeyPressed(const KeyCode key)
	{
		i32 state = glfwGetKey(static_cast<GLFWwindow*>(Application::Get()->GetNativeWindow()), static_cast<uint32_t>(key));
		return state == GLFW_PRESS;
	}

	bool Input::IsMouseButtonPressed(const MouseCode button)
	{
		i32 state = glfwGetMouseButton(static_cast<GLFWwindow*>(Application::Get()->GetNativeWindow()), static_cast<uint32_t>(button));
		return state == GLFW_PRESS;
	}

	glm::vec2 Input::GetMousePosition()
	{
		f64 xpos, ypos;
		glfwGetCursorPos(static_cast<GLFWwindow*>(Application::Get()->GetNativeWindow()), &xpos, &ypos);
		return { xpos, ypos };
	}
}