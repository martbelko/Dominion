#include "dmpch.h"
#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Dominion {

	OrthographicCamera::OrthographicCamera(F32 left, F32 right, F32 bottom, F32 top, F32 size)
		: m_ProjectionMatrix(glm::ortho(left * size, right * size, bottom * size, top * size, -1.0f, 1.0f)), m_ViewMatrix(1.0f),
		m_ViewProjectionMatrix(m_ProjectionMatrix * m_ViewMatrix), m_Size(size)
	{
		DM_PROFILE_FUNCTION();
	}

	void OrthographicCamera::SetProjection(F32 left, F32 right, F32 bottom, F32 top)
	{
		SetProjection(left, right, bottom, top, m_Size);
	}

	void OrthographicCamera::SetProjection(F32 left, F32 right, F32 bottom, F32 top, F32 size)
	{
		DM_PROFILE_FUNCTION();

		m_ProjectionMatrix = glm::ortho(left * size, right * size, bottom * size, top * size, -1.0f, 1.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		DM_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::rotate(glm::mat4(1.0f), m_Rotation, glm::vec3(0, 0, 1));

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

}
