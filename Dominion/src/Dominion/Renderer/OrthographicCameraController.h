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
		OrthographicCameraController(float aspectRatio = 1.0f, bool rotation = false);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		void OnResize(float width, float height);

		OrthographicCamera& GetCamera();
		const OrthographicCamera& GetCamera() const;

		float GetZoomLevel() const;
		void SetZoomLevel(float level);
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizedEvent& e);
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthographicCamera m_Camera;

		bool m_Rotation;

		float m_CameraTranslationSpeed = 5.0f, m_CameraRotationSpeed = 180.0f;
	};

}
