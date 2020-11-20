#include "dmpch.h"
#include "PerspectiveCameraController.h"

#include "Dominion/Core/Input.h"

#include "Dominion/Events/WindowEvent.h"
#include "Dominion/Events/MouseEvent.h"

namespace Dominion {

	PerspectiveCameraController::PerspectiveCameraController(float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio), m_Camera(glm::radians(45.0f), aspectRatio, 0.01f, 1000.0f), m_Rotation(rotation)
	{
		auto [x, y] = Input::GetMousePosition();
		m_LastX = x;
		m_LastY = y;
	}

	void PerspectiveCameraController::OnUpdate(Timestep ts)
	{
		DM_PROFILE_FUNCTION();

		glm::vec3& cameraPosition = m_Camera.m_Position;

		bool recalculate = false;
		if (Input::IsKeyPressed(Key::A))
		{
			glm::vec3 left = glm::normalize(glm::cross(m_Camera.m_Up, m_Camera.m_Front));
			m_Camera.m_Position += left * m_CameraTranslationSpeed;
			recalculate = true;
		}
		else if (Input::IsKeyPressed(Key::D))
		{
			glm::vec3 right = glm::normalize(glm::cross(m_Camera.m_Front, m_Camera.m_Up));
			m_Camera.m_Position += right * m_CameraTranslationSpeed;
			recalculate = true;
		}

		if (Input::IsKeyPressed(Key::W))
		{
			m_Camera.m_Position += m_Camera.m_Front * m_CameraTranslationSpeed;
			recalculate = true;
		}
		else if (Input::IsKeyPressed(Key::S))
		{
			m_Camera.m_Position -= m_Camera.m_Front * m_CameraTranslationSpeed;
			recalculate = true;
		}

		/*if (m_Rotation)
		{
			if (Input::IsKeyPressed(Key::Q))
				m_CameraRotation += m_CameraRotationSpeed * ts;
			if (Input::IsKeyPressed(Key::E))
				m_CameraRotation -= m_CameraRotationSpeed * ts;

			if (m_CameraRotation > 180.0f)
				m_CameraRotation -= 360.0f;
			else if (m_CameraRotation <= -180.0f)
				m_CameraRotation += 360.0f;

			m_Camera.SetRotation(m_CameraRotation);
		}*/

		if (recalculate)
			m_Camera.RecalculateViewMatrix();

		//m_CameraTranslationSpeed = m_ZoomLevel;
	}

	void PerspectiveCameraController::OnEvent(Event& e)
	{
		DM_PROFILE_FUNCTION();

		e.Dispatch<MouseMovedEvent>(DM_BIND_EVENT_FN(PerspectiveCameraController::OnMouseMoved));
		e.Dispatch<MouseScrolledEvent>(DM_BIND_EVENT_FN(PerspectiveCameraController::OnMouseScrolled));
		e.Dispatch<WindowResizedEvent>(DM_BIND_EVENT_FN(PerspectiveCameraController::OnWindowResized));
	}

	void PerspectiveCameraController::OnResize(float width, float height)
	{
		m_AspectRatio = width / height;
		m_Camera.SetProjection(glm::radians(45.0f), m_AspectRatio, 0.01f, 1000.0f);
	}

	bool PerspectiveCameraController::OnMouseMoved(MouseMovedEvent& e)
	{
		DM_PROFILE_FUNCTION();

		float xoffset = e.GetX() - m_LastX;
		float yoffset = e.GetY() - m_LastY;
		m_LastX = e.GetX();
		m_LastY = e.GetY();

		float sensitivity = 0.001f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		m_Camera.m_Yaw += xoffset;
		m_Camera.m_Pitch -= yoffset;

		if (m_Camera.m_Pitch > 89.0f)
			m_Camera.m_Pitch = 89.0f;
		if (m_Camera.m_Pitch < -89.0f)
			m_Camera.m_Pitch = -89.0f;

		glm::vec3 direction;
		direction.x = glm::cos(glm::radians(m_Camera.m_Yaw)) * glm::cos(glm::radians(m_Camera.m_Pitch));
		direction.y = glm::sin(glm::radians(m_Camera.m_Pitch));
		direction.z = glm::sin(glm::radians(m_Camera.m_Yaw)) * glm::cos(glm::radians(m_Camera.m_Pitch));
		m_Camera.m_Front = glm::normalize(direction);

		m_Camera.RecalculateViewMatrix();

		return false;
	}

	bool PerspectiveCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		DM_PROFILE_FUNCTION();

		float zoomLevel = m_Camera.m_Position.z - e.GetYOffset() * 0.25f;
		zoomLevel = std::max(zoomLevel, 0.25f);
		m_Camera.m_Position.z = zoomLevel;
		m_Camera.RecalculateViewMatrix();
		return false;
	}

	bool PerspectiveCameraController::OnWindowResized(WindowResizedEvent& e)
	{
		DM_PROFILE_FUNCTION();

		OnResize(static_cast<float>(e.GetWidth()), static_cast<float>(e.GetHeight()));
		return false;
	}

}
