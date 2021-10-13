#pragma once

#include "Dominion/Renderer/Camera.h"
#include "Dominion/Core/Timestep.h"
#include "Dominion/Events/Event.h"
#include "Dominion/Events/MouseEvent.h"

#include <glm/glm.hpp>

namespace Dominion {

	class EditorCamera
	{
	public:
		EditorCamera() = default;
		EditorCamera(F32 fov, F32 aspectRatio, F32 nearClip, F32 farClip);

		// TODO: Rule of 5

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		F32 GetDistance() const { return m_Distance; }
		void SetDistance(F32 distance) { m_Distance = distance; }

		void SetViewportSize(F32 width, F32 height) { m_ViewportWidth = width; m_ViewportHeight = height; UpdateProjection(); }

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		glm::mat4 GetViewProjectionMatrix() const { return m_ProjectionMatrix * m_ViewMatrix; }

		glm::vec3 GetUpDirection() const;
		glm::vec3 GetRightDirection() const;
		glm::vec3 GetForwardDirection() const;
		const glm::vec3& GetPosition() const { return m_Position; }
		glm::quat GetOrientation() const;

		F32 GetPitch() const { return m_Pitch; }
		F32 GetYaw() const { return m_Yaw; }
	private:
		void UpdateProjection();
		void UpdateView();

		bool OnMouseScroll(MouseScrolledEvent& e);

		void MousePan(const glm::vec2& delta);
		void MouseRotate(const glm::vec2& delta);
		void MouseZoom(F32 delta);

		glm::vec3 CalculatePosition() const;

		std::pair<F32, F32> PanSpeed() const;
		F32 RotationSpeed() const;
		F32 ZoomSpeed() const;
	private:
		F32 m_FOV = 45.0f, m_AspectRatio = 1.778f, m_NearClip = 0.1f, m_FarClip = 1000.0f;

		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };
		glm::vec2 m_LastMousePosition;

		F32 m_Distance = 10.0f;
		F32 m_Pitch = 0.0f, m_Yaw = 0.0f;

		F32 m_ViewportWidth = 1280, m_ViewportHeight = 720;
	};

}
