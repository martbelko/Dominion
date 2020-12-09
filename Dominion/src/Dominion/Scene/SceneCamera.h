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

		void SetPerspective(F32 verticalFOV, F32 nearClip, F32 farClip);

		void SetOrthographic(F32 size, F32 nearClip, F32 farClip);

		ProjectionType GetProjectionType() const;
		void SetProjectionType(ProjectionType type);

		// Perspective
		F32 GetPerspectiveFOV() const;
		void SetPerspectiveFOV(F32 verticalFOV);

		F32 GetPerspectiveNearClip() const;
		void SetPerspectiveNearClip(F32 nearClip);

		F32 GetPerspectiveFarClip() const;
		void SetPerspectiveFarClip(F32 farClip);

		// Orthographic
		F32 GetOrthographicSize() const;
		void SetOrthographicSize(F32 size);

		F32 GetOrthographicNearClip() const;
		void SetOrthographicNearClip(F32 nearClip);

		F32 GetOrthographicFarClip() const;
		void SetOrthographicFarClip(F32 farClip);
	private:
		void RecalculateProjection();
	private:
		ProjectionType m_ProjectionType = ProjectionType::Orthographic;

		F32 m_PerspectiveFOV = glm::radians(45.0f);
		F32 m_PerspectiveNear = 0.01f, m_PerspectiveFar = 1000.0f;

		F32 m_OrthographicSize = 10.0f;
		F32 m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;

		F32 m_AspectRatio = 1.0f;
	};

}
