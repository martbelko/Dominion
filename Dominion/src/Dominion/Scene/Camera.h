#pragma once

#include <glm/glm.hpp>

namespace Dominion {

	class Camera
	{
	public:
		virtual ~Camera() = default;

		virtual const glm::mat4& GetProjectionMatrix() const = 0;
		virtual const glm::mat4& GetViewMatrix() const = 0;
		virtual const glm::mat4& GetViewProjectionMatrix() const = 0;

		virtual const glm::vec3& GetPosition() const = 0;
		virtual void SetPosition(const glm::vec3& position) = 0;

		virtual float GetRotation() const = 0;
		virtual void SetRotation(float rotation) = 0;
	};

	class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		void SetProjection(float left, float right, float bottom, float top);

		virtual const glm::vec3& GetPosition() const override;
		virtual void SetPosition(const glm::vec3& position) override;

		virtual float GetRotation() const override;
		virtual void SetRotation(float rotation) override;

		virtual const glm::mat4& GetProjectionMatrix() const override;
		virtual const glm::mat4& GetViewMatrix() const override;
		virtual const glm::mat4& GetViewProjectionMatrix() const override;
	private:
		void RecalculateViewMatrix();
	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		float m_Rotation = 0.0f;
	};
}
