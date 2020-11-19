#pragma once

#include "Dominion/Core/Base.h"
#include "Dominion/Renderer/PerspectiveCamera.h"
#include "Dominion/Core/Timestep.h"

#include <glm/glm.hpp>

namespace Dominion {

	// Forward declarations
	class Event;
	class MouseMovedEvent;
	class MouseScrolledEvent;
	class WindowResizedEvent;

	class PerspectiveCameraController
	{
	public:
		PerspectiveCameraController(float aspectRatio = 1.0f, bool rotation = false);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		void OnResize(float width, float height);

		PerspectiveCamera& GetCamera() { return m_Camera; }
		const PerspectiveCamera& GetCamera() const { return m_Camera; }

		float GetZoomLevel() const { return m_Camera.m_Position.z; }
		void SetZoomLevel(float level) { m_Camera.m_Position.z = level; m_Camera.RecalculateViewMatrix(); }
	private:
		bool OnMouseMoved(MouseMovedEvent& e);
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizedEvent& e);
	private:
		float m_AspectRatio;
		PerspectiveCamera m_Camera;

		float m_LastX, m_LastY;

		bool m_Rotation;

		float m_CameraTranslationSpeed = 0.1f, m_CameraRotationSpeed = 180.0f;
	};

}
