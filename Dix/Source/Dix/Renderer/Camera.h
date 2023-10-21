#pragma once

#include "Dix/Core/Base.h"
#include "Dix/Core/Timestep.h"

#include "Dix/Events/ApplicationEvent.h"
#include "Dix/Events/MouseEvent.h"

#include <glm/glm.hpp>

namespace Dix
{
	class Camera
	{
	public:
		Camera() = default;
		Camera(f32 fov, f32 aspectRatio, f32 nearClip, f32 farClip);

		glm::vec3 GetPosition() const { return m_Position; }

		glm::mat4 GetViewMatrix() const { return m_ViewMatrix; }
		glm::mat4 GetProjectionMatrix() const { return m_ProjectionMatrix; }
		glm::mat4 GetViewProjectionMatrix() const { return m_ProjectionMatrix * m_ViewMatrix; }

		void OnUpdate(Timestep ts);
		void OnImGuiRender();
		void OnEvent(Event& event);

	private:
		void UpdateProjection();
		void UpdateView();

		bool OnWindowResize(WindowResizeEvent& event);
		bool OnMouseScroll(MouseScrolledEvent& event);

	private:
		f32 m_FOV = 45.0f, m_AspectRatio = 1.0f, m_Near = 0.1f, m_Far = 100.0f;

		f32 m_Pitch = 0.0f, m_Yaw = -90.0f;

		f32 m_Speed = 1.5f, m_Sensetivity = 0.1f;

		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;

		glm::vec3 m_Position = { 0.0f, 0.0f, 3.0f };
		glm::vec3 m_Forward	 = { 0.0f, 0.0f, -1.0f };
		glm::vec3 m_Right    = { 1.0f, 0.0f, 0.0f };
		glm::vec3 m_Up		 = { 0.0f, 1.0f, 0.0f };

		glm::vec2 m_LastMousePosition = { 0.0f, 0.0f };
	};
}