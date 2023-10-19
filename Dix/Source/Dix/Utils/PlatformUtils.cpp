#include "dixpch.h"
#include "Dix/Utils/PlatformUtils.h"

#include <GLFW/glfw3.h>

namespace Dix
{
	f32 Time::GetTime()
	{
		return glfwGetTime();
	}
}