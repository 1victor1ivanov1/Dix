#include "dixpch.h"
#include "Dix/Utils/PlatformUtils.h"

#include <GLFW/glfw3.h>

namespace Dix
{
	float Time::GetTime()
	{
		return glfwGetTime();
	}
}