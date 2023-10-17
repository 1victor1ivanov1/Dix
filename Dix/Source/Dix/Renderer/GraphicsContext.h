#pragma once

#include "Dix/Core/Base.h"

struct GLFWwindow;

namespace Dix
{
	class GraphicsContext
	{
		public:
			GraphicsContext(GLFWwindow* windowHandle);
			
			void SwapBuffers();

			static UniquePtr<GraphicsContext> Create(GLFWwindow* windowHandle) { return MakeUnique<GraphicsContext>(windowHandle); }
		private:
			void Init();
		private:
			GLFWwindow* m_WindowHandle;
	};
}