#pragma once

#include "Dominion/Core/Base.h"

#include <glm/glm.hpp>

namespace Dominion {

	class OrthographicCamera
	{
	public:
		OrthographicCamera(F32 left = -1.0f, F32 right = 1.0f, F32 bottom = -1.0f, F32 top = 1.0f);

		void SetProjection(F32 left, F32 right, F32 bottom, F32 top);

		const glm::vec3& GetPosition() const;
		glm::vec3& GetPosition() { return m_Position; }
		void SetPosition(const glm::vec3& position);

		F32 GetRotation() const;
		F32& GetRotation() { return m_Rotation; }
		void SetRotation(F32 rotation);

		const glm::mat4& GetProjectionMatrix() const;
		const glm::mat4& GetViewMatrix() const;
		const glm::mat4& GetViewProjectionMatrix() const;
	private:
		void RecalculateViewMatrix();
	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		F32 m_Rotation = 0.0f; // In radians

		friend class OrthographicCameraController;
	};

}
