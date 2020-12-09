#pragma once

#include "Dominion/Core/Base.h"

#include <glm/glm.hpp>

namespace Dominion {

	class PerspectiveCamera
	{
	public:
		PerspectiveCamera(F32 verticalFOV = glm::radians(45.0f), F32 aspectRatio = 1.0f, F32 nearClip = 0.01f, F32 farClip = 1000.0f);

		void SetProjection(F32 verticalFOV, F32 aspectRatio, F32 nearClip, F32 farClip);

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

		glm::vec3 CreateRay();
	private:
		void RecalculateViewMatrix();
	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Front{ 0.0f, 0.0f, -1.0f };
		glm::vec3 m_Up{ 0.0f, 1.0f, 0.0f };
		F32 m_Yaw = glm::radians(-90.0f);
		F32 m_Pitch = glm::radians(0.0f);

		friend class PerspectiveCameraController;
	};

}
