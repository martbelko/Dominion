#include "dmpch.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Dominion {

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(1.0), m_ViewProjectionMatrix(m_ProjectionMatrix * m_ViewMatrix)
	{
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	const glm::vec3& OrthographicCamera::GetPosition() const
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

}
