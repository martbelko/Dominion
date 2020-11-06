#include "dmpch.h"
#include "Camera.h"

#include "Dominion/Core/Input.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Dominion {

	OrthographicCamera::OrthographicCamera(float aspectRatio, float zoomLevel)
		: m_ProjectionMatrix(glm::ortho(-aspectRatio * zoomLevel, aspectRatio * zoomLevel, -1.0f, 1.0f, -1.0f, 1.0f)),
		  m_ViewMatrix(1.0), m_ViewProjectionMatrix(m_ProjectionMatrix* m_ViewMatrix),
		  m_AspectRatio(aspectRatio), m_ZoomLevel(zoomLevel)
	{

	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::OnUpdate(const Timestep& timestep)
	{
		if (Input::IsKeyPressed(Dominion::Key::W))
		{
			m_Position.y += m_ZoomLevel * timestep;
		}
		else if (Input::IsKeyPressed(Dominion::Key::S))
		{
			m_Position.y -= m_ZoomLevel * timestep;
		}

		if (Input::IsKeyPressed(Dominion::Key::A))
		{
			m_Position.x -= m_ZoomLevel * timestep;
		}
		else if (Input::IsKeyPressed(Dominion::Key::D))
		{
			m_Position.x += m_ZoomLevel * timestep;
		}
	}

	void OrthographicCamera::OnEvent(Event& e)
	{
		e.Dispatch<WindowResizedEvent>(DM_BIND_EVENT_FN(OrthographicCamera::OnWindowResize));
		e.Dispatch<MouseScrolledEvent>(DM_BIND_EVENT_FN(OrthographicCamera::OnMouseScrolled));
	}

	void OrthographicCamera::Refresh()
	{
		RecalculateViewMatrix();
	}

	const glm::vec3& OrthographicCamera::GetPosition() const
	{
		return m_Position;
	}

	glm::vec3& OrthographicCamera::GetPosition()
	{
		return m_Position;
	}

	void OrthographicCamera::SetPosition(const glm::vec3& position)
	{
		m_Position = position;
	}

	float OrthographicCamera::GetRotation() const
	{
		return m_Rotation;
	}

	float& OrthographicCamera::GetRotation()
	{
		return m_Rotation;
	}

	void OrthographicCamera::SetRotation(float rotation)
	{
		m_Rotation = rotation;
	}

	const glm::mat4& OrthographicCamera::GetProjectionMatrix() const
	{
		return m_ProjectionMatrix;
	}

	const glm::mat4& OrthographicCamera::GetViewMatrix() const
	{
		return m_ViewMatrix;
	}

	const glm::mat4& OrthographicCamera::GetViewProjectionMatrix() const
	{
		return m_ViewProjectionMatrix;
	}

	bool OrthographicCamera::OnWindowResize(WindowResizedEvent& e)
	{
		m_AspectRatio = static_cast<float>(e.GetWidth()) / static_cast<float>(e.GetHeight());
		SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthographicCamera::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset();
		m_ZoomLevel = std::clamp(m_ZoomLevel, 1.0f, 20.0f);
		SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

}
