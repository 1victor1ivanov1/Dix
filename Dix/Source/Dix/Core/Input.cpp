#include "dixpch.h"

#include "Dix/Core/Input.h"
#include "Dix/Core/Application.h"

#include <GLFW/glfw3.h>

namespace Dix
{
	bool Input::IsKeyPressed(const KeyCode key)
	{
		int state = glfwGetKey(static_cast<GLFWwindow*>(Application::Get()->GetNativeWindow()), static_cast<uint32_t>(key));
		return state == GLFW_PRESS;
	}

	bool Input::IsMouseButtonPressed(const MouseCode button)
	{
		int state = glfwGetMouseButton(static_cast<GLFWwindow*>(Application::Get()->GetNativeWindow()), static_cast<uint32_t>(button));
		return state == GLFW_PRESS;
	}

	//glm::vec2 Input::GetMousePosition()
	//{
	//	double xpos, ypos;
	//	glfwGetCursorPos(static_cast<GLFWwindow*>(Application::Get()->GetNativeWindow()), &xpos, &ypos);
	//	return { xpos, ypos };
	//}
}