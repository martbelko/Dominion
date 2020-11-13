#pragma once

#include "Dominion/Core/Base.h"
#include "Dominion/Renderer/Camera.h"

namespace Dominion {

	class SceneCamera : public Camera
	{
	public:
		enum class ProjectionType { Perspective = 0, Orthographic = 1 };
	public:
		SceneCamera();
		virtual ~SceneCamera() override = default;

		void SetViewportSize(uint32_t width, uint32_t height);

		void SetPerspective(float verticalFOV, float nearClip, float farClip);

		void SetOrthographic(float size, float nearClip, float farClip);

		ProjectionType GetProjectionType() const;
		void SetProjectionType(ProjectionType type);

		// Perspective
		float GetPerspectiveFOV() const;
		void SetPerspectiveFOV(float verticalFOV);

		float GetPerspectiveNearClip() const;
		void SetPerspectiveNearClip(float nearClip);

		float GetPerspectiveFarClip() const;
		void SetPerspectiveFarClip(float farClip);

		// Orthographic
		float GetOrthographicSize() const;
		void SetOrthographicSize(float size);

		float GetOrthographicNearClip() const;
		void SetOrthographicNearClip(float nearClip);

		float GetOrthographicFarClip() const;
		void SetOrthographicFarClip(float farClip);
	private:
		void RecalculateProjection();
	private:
		ProjectionType m_ProjectionType = ProjectionType::Orthographic;

		float m_PerspectiveFOV = glm::radians(45.0f);
		float m_PerspectiveNear = 0.01f, m_PerspectiveFar = 1000.0f;

		float m_OrthographicSize = 10.0f;
		float m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;

		float m_AspectRatio = 1.0f;
	};

}
