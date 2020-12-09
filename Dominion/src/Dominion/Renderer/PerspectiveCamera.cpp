#include "dmpch.h"
#include "PerspectiveCamera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Dominion {

	PerspectiveCamera::PerspectiveCamera(F32 verticalFOV, F32 aspectRatio, F32 nearClip, F32 farClip)
		: m_ProjectionMatrix(glm::perspective(verticalFOV, aspectRatio, nearClip, farClip)), m_ViewMatrix(1.0f), m_ViewProjectionMatrix(m_ProjectionMatrix * m_ViewMatrix)
	{
		DM_PROFILE_FUNCTION();
	}

	void PerspectiveCamera::SetProjection(F32 verticalFOV, F32 aspectRatio, F32 nearClip, F32 farClip)
	{
		DM_PROFILE_FUNCTION();

		m_ProjectionMatrix = glm::perspective(verticalFOV, aspectRatio, nearClip, farClip);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	glm::vec3 PerspectiveCamera::CreateRay()
	{
		return m_Front;
	}

	void PerspectiveCamera::RecalculateViewMatrix()
	{
		DM_PROFILE_FUNCTION();

		glm::vec3 front;
		front.x = glm::cos(m_Yaw) * glm::cos(m_Pitch);
		front.y = glm::sin(m_Pitch);
		front.z = glm::sin(m_Yaw) * glm::cos(m_Pitch);
		m_Front = glm::normalize(front);
		glm::vec3 right = glm::normalize(glm::cross(m_Front, glm::vec3(0.0f, 1.0f, 0.0f)));
		m_Up = glm::normalize(glm::cross(right, m_Front));

		m_ViewProjectionMatrix = m_ProjectionMatrix * glm::lookAt(m_Position, m_Position + m_Front, m_Up);
	}

}
