#pragma once

#include "Dominion/Core/Timestep.h"

#include "Dominion/Events/ApplicationEvent.h"
#include "Dominion/Events/MouseEvent.h"

#include "Dominion/Renderer/OrthographicCamera.h"

namespace Dominion {

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		void OnResize(float width, float height);

		OrthographicCamera& GetCamera() { return mCamera; }
		const OrthographicCamera& GetCamera() const { return mCamera; }

		float GetZoomLevel() const { return mZoomLevel; }
		void SetZoomLevel(float level) { mZoomLevel = level; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		float mAspectRatio;
		float mZoomLevel = 1.0f;
		OrthographicCamera mCamera;

		bool mRotation;

		glm::vec3 mCameraPosition = { 0.0f, 0.0f, 0.0f };
		float mCameraRotation = 0.0f; //In degrees, in the anti-clockwise direction
		float mCameraTranslationSpeed = 5.0f, mCameraRotationSpeed = 180.0f;
	};

}
