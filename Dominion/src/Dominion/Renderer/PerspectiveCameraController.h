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
		PerspectiveCameraController(F32 aspectRatio = 1.0f, bool rotation = false);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		void OnResize(F32 width, F32 height);

		PerspectiveCamera& GetCamera() { return m_Camera; }
		const PerspectiveCamera& GetCamera() const { return m_Camera; }

		F32 GetZoomLevel() const { return m_Camera.m_Position.z; }
		void SetZoomLevel(F32 level) { m_Camera.m_Position.z = level; m_Camera.RecalculateViewMatrix(); }
	private:
		bool OnMouseMoved(MouseMovedEvent& e);
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizedEvent& e);
	private:
		F32 m_AspectRatio;
		PerspectiveCamera m_Camera;

		F32 m_LastX, m_LastY;

		bool m_Rotation;

		F32 m_CameraTranslationSpeed = 0.1f, m_CameraRotationSpeed = 180.0f;
	};

}
