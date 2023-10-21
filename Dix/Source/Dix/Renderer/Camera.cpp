#include "dixpch.h"

#include "Dix/Core/Input.h"

#include "Dix/Renderer/Camera.h"

#include <imgui.h>

namespace Dix
{
	Dix::Camera::Camera(f32 fov, f32 aspectRatio, f32 nearClip, f32 farClip)
		: m_FOV(fov), m_AspectRatio(aspectRatio), m_Near(nearClip), m_Far(farClip)
	{
		UpdateProjection();
		UpdateView();

		m_LastMousePosition = Input::GetMousePosition();
	}

	void Camera::OnUpdate(Timestep ts)
	{
		{
			if (Input::IsKeyPressed(Key::W))
			{
				m_Position += m_Forward * m_Speed * ts.GetSeconds();
			}
			if (Input::IsKeyPressed(Key::A))
			{
				m_Position -= m_Right * m_Speed * ts.GetSeconds();
			}
			if (Input::IsKeyPressed(Key::S))
			{
				m_Position -= m_Forward * m_Speed * ts.GetSeconds();
			}
			if (Input::IsKeyPressed(Key::D))
			{
				m_Position += m_Right * m_Speed * ts.GetSeconds();
			}
			if (Input::IsKeyPressed(Key::Q))
			{
				m_Position += m_Up * m_Speed * ts.GetSeconds();
			}
			if (Input::IsKeyPressed(Key::E))
			{
				m_Position -= m_Up * m_Speed * ts.GetSeconds();
			}
		}

		{
			if (Input::IsKeyPressed(Key::LeftAlt))
			{
				glm::vec2 currentMousePosition = Input::GetMousePosition();
				glm::vec2 mouseOffset = currentMousePosition - m_LastMousePosition;
				m_LastMousePosition = currentMousePosition;

				mouseOffset *= m_Sensetivity;

				m_Yaw += mouseOffset.x;
				m_Pitch -= mouseOffset.y;

				if (m_Pitch > 89.0f)
					m_Pitch = 89.0f;
				if (m_Pitch < -89.0f)
					m_Pitch = -89.0f;

				glm::vec3 direction;
				direction.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
				direction.y = sin(glm::radians(m_Pitch));
				direction.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
				m_Forward = glm::normalize(direction);
				m_Right = glm::cross(m_Forward, m_Up);
			}
			m_LastMousePosition = Input::GetMousePosition();
		}

		UpdateView();
	}

	void Camera::OnImGuiRender()
	{
		ImGui::Begin("Camera control");

		ImGui::SliderFloat("Camera Speed", &m_Speed, 0.0f, 10.0f);
		ImGui::SliderFloat("Camera Sensetivity", &m_Sensetivity, 0.0f, 1.0f);

		ImGui::End();
	}

	void Camera::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);

		dispatcher.Dispatch<WindowResizeEvent>(DIX_BIND_EVENT_CALLBACK(Camera::OnWindowResize));
		dispatcher.Dispatch<MouseScrolledEvent>(DIX_BIND_EVENT_CALLBACK(Camera::OnMouseScroll));
	}

	void Camera::UpdateProjection()
	{
		m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_Near, m_Far);
	}

	void Camera::UpdateView()
	{
		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Forward, m_Up);
	}

	bool Camera::OnWindowResize(WindowResizeEvent& event)
	{
		m_AspectRatio = static_cast<f32>(event.GetWidth()) / event.GetHeight();
		UpdateProjection();

		return false;
	}

	bool Camera::OnMouseScroll(MouseScrolledEvent& event)
	{
		m_FOV -= event.GetOffsetY();
		if (m_FOV < 1.0f)
			m_FOV = 1.0f;
		if (m_FOV > 45.0f)
			m_FOV = 45.0f;
		UpdateProjection();

		return false;
	}
}