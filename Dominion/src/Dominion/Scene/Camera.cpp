#include "dmpch.h"
#include "Camera.h"

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

	void OrthographicCamera::OnEvent(Event& e)
	{
		e.Dispatch<WindowResizedEvent>(DM_BIND_EVENT_FN(OrthographicCamera::OnWindowResize));
		e.Dispatch<MouseScrolledEvent>(DM_BIND_EVENT_FN(OrthographicCamera::OnMouseScrolled));
	}

	const glm::vec3& OrthographicCamera::GetPosition() const
	{
		return m_Position;
	}

	void OrthographicCamera::SetPosition(const glm::vec3& position)
	{
		m_Position = position;
		RecalculateViewMatrix();
	}

	float OrthographicCamera::GetRotation() const
	{
		return m_Rotation;
	}

	void OrthographicCamera::SetRotation(float rotation)
	{
		m_Rotation = rotation;
		RecalculateViewMatrix();
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
