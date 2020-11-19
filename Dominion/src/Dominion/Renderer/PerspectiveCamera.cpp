#include "dmpch.h"
#include "PerspectiveCamera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Dominion {

	PerspectiveCamera::PerspectiveCamera(float verticalFOV, float aspectRatio, float nearClip, float farClip)
		: m_ProjectionMatrix(glm::perspective(verticalFOV, aspectRatio, nearClip, farClip)), m_ViewMatrix(1.0f), m_ViewProjectionMatrix(m_ProjectionMatrix * m_ViewMatrix)
	{
		DM_PROFILE_FUNCTION();
	}

	void PerspectiveCamera::SetProjection(float verticalFOV, float aspectRatio, float nearClip, float farClip)
	{
		DM_PROFILE_FUNCTION();

		m_ProjectionMatrix = glm::perspective(verticalFOV, aspectRatio, nearClip, farClip);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void PerspectiveCamera::RecalculateViewMatrix()
	{
		DM_PROFILE_FUNCTION();

		glm::vec3 front;
		front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		front.y = sin(glm::radians(m_Pitch));
		front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		m_Front = glm::normalize(front);
		glm::vec3 right = glm::normalize(glm::cross(m_Front, glm::vec3(0.0f, 1.0f, 0.0f)));
		m_Up = glm::normalize(glm::cross(right, m_Front));

		m_ViewProjectionMatrix = m_ProjectionMatrix * glm::lookAt(m_Position, m_Position + m_Front, m_Up);
		/*glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position);

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;*/
	}

}
