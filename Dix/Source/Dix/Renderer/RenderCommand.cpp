#include "dixpch.h"

#include "Dix/Renderer/RenderCommand.h"
#include "Dix/Renderer/Buffer.h"

#include <glad/glad.h>

namespace Dix
{
	static void APIENTRY glDebugOutput(GLenum source,
		GLenum type,
		u32 id,
		GLenum severity,
		GLsizei length,
		const char* message,
		const void* userParam)
	{
		// ignore non-significant error/warning codes
		if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

		std::string debugSource;
		switch (source)
		{
		case GL_DEBUG_SOURCE_API:				debugSource = "Source: API"; break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:		debugSource = "Source: Window System"; break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER:	debugSource = "Source: Shader Compiler"; break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:		debugSource = "Source: Third Party"; break;
		case GL_DEBUG_SOURCE_APPLICATION:		debugSource = "Source: Application"; break;
		case GL_DEBUG_SOURCE_OTHER:				debugSource = "Source: Other"; break;
		}

		std::string debugType;
		switch (type)
		{
		case GL_DEBUG_TYPE_ERROR:               debugType = "Type: Error"; break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: debugType = "Type: Deprecated Behaviour"; break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  debugType = "Type: Undefined Behaviour"; break;
		case GL_DEBUG_TYPE_PORTABILITY:         debugType = "Type: Portability"; break;
		case GL_DEBUG_TYPE_PERFORMANCE:         debugType = "Type: Performance"; break;
		case GL_DEBUG_TYPE_MARKER:              debugType = "Type: Marker"; break;
		case GL_DEBUG_TYPE_PUSH_GROUP:			debugType = "Type: Push Group"; break;
		case GL_DEBUG_TYPE_POP_GROUP:           debugType = "Type: Pop Group"; break;
		case GL_DEBUG_TYPE_OTHER:               debugType = "Type: Other"; break;
		}

		std::string debugSeverity;
		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:			DIX_CORE_CRITICAL("{0}, {1} - {2}", debugSource, debugType, message); break;
		case GL_DEBUG_SEVERITY_MEDIUM:			DIX_CORE_ERROR("{0}, {1} - {2}", debugSource, debugType, message); break;
		case GL_DEBUG_SEVERITY_LOW:				DIX_CORE_WARN("{0}, {1} - {2}", debugSource, debugType, message); break;
		case GL_DEBUG_SEVERITY_NOTIFICATION:	DIX_CORE_TRACE("{0}, {1} - {2}", debugSource, debugType, message); break;
		}
	}

	void RenderCommand::Init()
	{
#ifdef DIX_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
#endif

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//glEnable(GL_DEPTH_TEST);
		glEnable(GL_LINE_SMOOTH);
	}

	void RenderCommand::SetViewport(u32 x, u32 y, u32 width, u32 height)
	{
		glViewport(x, y, width, height);
	}

	void RenderCommand::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void RenderCommand::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void RenderCommand::EnableDepthTest()
	{
		glEnable(GL_DEPTH_TEST);
	}

	void RenderCommand::DisableDepthTest()
	{
		glDisable(GL_DEPTH_TEST);
	}

	void RenderCommand::DrawIndexed(const SharedPtr<VertexArray>& vertexArray, u32 indexCount)
	{
		vertexArray->Bind();
		u32 count = indexCount == 0 ? vertexArray->GetIndexBuffer()->GetCount() : indexCount;
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
		vertexArray->Unbind();
	}

	void RenderCommand::DrawArrays(const SharedPtr<VertexArray>& vertexArray, u32 vertexCount)
	{
		vertexArray->Bind();
		glDrawArrays(GL_TRIANGLES, 0, vertexCount);
		vertexArray->Unbind();
	}
}