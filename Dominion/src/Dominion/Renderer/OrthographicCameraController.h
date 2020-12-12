#pragma once

#include "Dominion/Core/Base.h"

#include "Dominion/Renderer/OrthographicCamera.h"
#include "Dominion/Core/Timestep.h"

#include "Dominion/Events/WindowEvent.h"
#include "Dominion/Events/MouseEvent.h"

namespace Dominion {

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(F32 aspectRatio = 1.0f, bool rotation = false);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		void OnResize(F32 width, F32 height);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

		F32 GetZoomLevel() const { return m_ZoomLevel; }
		void SetZoomLevel(F32 level) { m_ZoomLevel = level; } // TODO: Not recalculate? Maybe recalculate. needs check

		F32 GetWidth() const { return 2 * m_AspectRatio * m_ZoomLevel; }
		F32 GetHeight() const { return 2 * m_ZoomLevel; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizedEvent& e);
	private:
		F32 m_AspectRatio;
		F32 m_ZoomLevel = 1.0f;
		OrthographicCamera m_Camera;

		bool m_Rotation;

		F32 m_CameraTranslationSpeed = 5.0f, m_CameraRotationSpeed = 180.0f;
	};

}
