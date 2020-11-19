#pragma once

#include "Dominion/Core/Base.h"

#include <glm/glm.hpp>

namespace Dominion {

	class PerspectiveCamera
	{
	public:
		PerspectiveCamera(float verticalFOV = glm::radians(45.0f), float aspectRatio = 1.0f, float nearClip = 0.01f, float farClip = 1000.0f);

		void SetProjection(float verticalFOV, float aspectRatio, float nearClip, float farClip);

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
	private:
		void RecalculateViewMatrix();
	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Front{ 0.0f, 0.0f, -1.0f };
		glm::vec3 m_Up{ 0.0f, 1.0f, 0.0f };
		float m_Yaw = -90.0f;
		float m_Pitch = 0.0f;

		friend class PerspectiveCameraController;
	};

}
