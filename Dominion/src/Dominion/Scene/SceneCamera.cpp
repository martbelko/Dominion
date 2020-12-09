#include "dmpch.h"
#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Dominion {

	SceneCamera::SceneCamera()
	{
		RecalculateProjection();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_AspectRatio = static_cast<F32>(width) / static_cast<F32>(height);
		RecalculateProjection();
	}

	void SceneCamera::SetPerspective(F32 verticalFOV, F32 nearClip, F32 farClip)
	{
		m_ProjectionType = ProjectionType::Perspective;
		m_PerspectiveFOV = verticalFOV;
		m_PerspectiveNear = nearClip;
		m_PerspectiveFar = farClip;

		RecalculateProjection();
	}

	void SceneCamera::SetOrthographic(F32 size, F32 nearClip, F32 farClip)
	{
		m_ProjectionType = ProjectionType::Orthographic;
		m_OrthographicSize = size;
		m_OrthographicNear = nearClip;
		m_OrthographicFar = farClip;

		RecalculateProjection();
	}

	SceneCamera::ProjectionType SceneCamera::GetProjectionType() const
	{
		return m_ProjectionType;
	}

	void SceneCamera::SetProjectionType(ProjectionType type)
	{
		m_ProjectionType = type;
		RecalculateProjection();
	}

	F32 SceneCamera::GetPerspectiveFOV() const
	{
		return m_PerspectiveFOV;
	}

	void SceneCamera::SetPerspectiveFOV(F32 verticalFOV)
	{
		m_PerspectiveFOV = verticalFOV;
		RecalculateProjection();
	}

	F32 SceneCamera::GetPerspectiveNearClip() const
	{
		return m_PerspectiveNear;
	}

	void SceneCamera::SetPerspectiveNearClip(F32 nearClip)
	{
		m_PerspectiveNear = nearClip;
		RecalculateProjection();
	}

	F32 SceneCamera::GetPerspectiveFarClip() const
	{
		return m_PerspectiveFar;
	}

	void SceneCamera::SetPerspectiveFarClip(F32 farClip)
	{
		m_PerspectiveFar = farClip;
		RecalculateProjection();
	}

	F32 SceneCamera::GetOrthographicSize() const
	{
		return m_OrthographicSize;
	}

	void SceneCamera::SetOrthographicSize(F32 size)
	{
		m_OrthographicSize = size;
		RecalculateProjection();
	}

	F32 SceneCamera::GetOrthographicNearClip() const
	{
		return m_OrthographicNear;
	}

	void SceneCamera::SetOrthographicNearClip(F32 nearClip)
	{
		m_OrthographicNear = nearClip;
		RecalculateProjection();
	}

	F32 SceneCamera::GetOrthographicFarClip() const
	{
		return m_OrthographicFar;
	}

	void SceneCamera::SetOrthographicFarClip(F32 farClip)
	{
		m_OrthographicFar = farClip;
		RecalculateProjection();
	}

	void SceneCamera::RecalculateProjection()
	{
		if (m_ProjectionType == ProjectionType::Perspective)
		{
			m_Projection = glm::perspective(m_PerspectiveFOV, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
		}
		else
		{
			F32 orthoLeft = -m_OrthographicSize * m_AspectRatio * 0.5f;
			F32 orthoRight = m_OrthographicSize * m_AspectRatio * 0.5f;
			F32 orthoBottom = -m_OrthographicSize * 0.5f;
			F32 orthoTop = m_OrthographicSize * 0.5f;

			m_Projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_OrthographicNear, m_OrthographicFar);
		}

	}

}
