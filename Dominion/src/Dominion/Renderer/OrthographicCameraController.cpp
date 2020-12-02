#include "dmpch.h"
#include "OrthographicCameraController.h"

#include "Dominion/Core/Input.h"
#include "Dominion/Core/KeyCodes.h"

namespace Dominion {

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_Rotation(rotation)
	{

	}

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		DM_PROFILE_FUNCTION();

		glm::vec3& cameraPosition = m_Camera.GetPosition();
		float& cameraRotation = m_Camera.GetRotation();

		bool recalculate = false;
		if (Input::IsKeyPressed(Key::A))
		{
			cameraPosition.x -= cos(cameraRotation) * m_CameraTranslationSpeed * ts;
			cameraPosition.y -= sin(cameraRotation) * m_CameraTranslationSpeed * ts;
			recalculate = true;
		}
		else if (Input::IsKeyPressed(Key::D))
		{
			cameraPosition.x += cos(cameraRotation) * m_CameraTranslationSpeed* ts;
			cameraPosition.y += sin(cameraRotation) * m_CameraTranslationSpeed* ts;
			recalculate = true;
		}

		if (Input::IsKeyPressed(Key::W))
		{
			cameraPosition.x += -sin(cameraRotation) * m_CameraTranslationSpeed * ts;
			cameraPosition.y += cos(cameraRotation) * m_CameraTranslationSpeed * ts;
			recalculate = true;
		}
		else if (Input::IsKeyPressed(Key::S))
		{
			cameraPosition.x -= -sin(cameraRotation) * m_CameraTranslationSpeed * ts;
			cameraPosition.y -= cos(cameraRotation) * m_CameraTranslationSpeed * ts;
			recalculate = true;
		}

		if (m_Rotation)
		{
			if (Input::IsKeyPressed(Key::Q))
			{
				cameraRotation += m_CameraRotationSpeed * ts;
				recalculate = true;
			}
			if (Input::IsKeyPressed(Key::E))
			{
				cameraRotation -= m_CameraRotationSpeed * ts;
				recalculate = true;
			}

			if (cameraRotation > 180.0f)
				cameraRotation -= 360.0f;
			else if (cameraRotation <= -180.0f)
				cameraRotation += 360.0f;
		}

		m_CameraTranslationSpeed = m_ZoomLevel;

		if (recalculate)
			m_Camera.RecalculateViewMatrix();
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		DM_PROFILE_FUNCTION();

		e.Dispatch<MouseScrolledEvent>(DM_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		e.Dispatch<WindowResizedEvent>(DM_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	void OrthographicCameraController::OnResize(float width, float height)
	{
		m_AspectRatio = width / height;
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}

	OrthographicCamera& OrthographicCameraController::GetCamera()
	{
		return m_Camera;
	}

	const OrthographicCamera& OrthographicCameraController::GetCamera() const
	{
		return m_Camera;
	}

	float OrthographicCameraController::GetZoomLevel() const
	{
		return m_ZoomLevel;
	}

	void OrthographicCameraController::SetZoomLevel(float level)
	{
		m_ZoomLevel = level;
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		DM_PROFILE_FUNCTION();

		m_ZoomLevel -= e.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizedEvent& e)
	{
		DM_PROFILE_FUNCTION();

		OnResize(static_cast<float>(e.GetWidth()), static_cast<float>(e.GetHeight()));
		return false;
	}

}
