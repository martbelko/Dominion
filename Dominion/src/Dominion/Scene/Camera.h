#pragma once

#include "Dominion/Events/Event.h"
#include "Dominion/Events/WindowEvent.h"
#include "Dominion/Events/MouseEvent.h"
#include "Dominion/Core/Timestep.h"

#include <glm/glm.hpp>

namespace Dominion {

	class Camera
	{
	public:
		virtual ~Camera() = default;

		virtual void OnUpdate(const Timestep& timestep) { };
		virtual void OnEvent(Event& event) { }

		virtual void Refresh() = 0;

		virtual const glm::mat4& GetProjectionMatrix() const = 0;
		virtual const glm::mat4& GetViewMatrix() const = 0;
		virtual const glm::mat4& GetViewProjectionMatrix() const = 0;

		virtual const glm::vec3& GetPosition() const = 0;
		virtual void SetPosition(const glm::vec3& position) = 0;

		virtual float GetRotation() const = 0;
		virtual void SetRotation(float rotation) = 0;
	};

	class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera() = default;
		OrthographicCamera(float aspectRatio, float zoomLevel = 1.0f);

		void SetProjection(float left, float right, float bottom, float top);

		virtual void OnUpdate(const Timestep& timestep) override;
		virtual void OnEvent(Event& e) override;

		virtual void Refresh() override;

		virtual const glm::vec3& GetPosition() const override;
		virtual void SetPosition(const glm::vec3& position) override;

		virtual float GetRotation() const override;
		virtual void SetRotation(float rotation) override;

		virtual const glm::mat4& GetProjectionMatrix() const override;
		virtual const glm::mat4& GetViewMatrix() const override;
		virtual const glm::mat4& GetViewProjectionMatrix() const override;
	private:
		bool OnWindowResize(WindowResizedEvent& e);
		bool OnMouseScrolled(MouseScrolledEvent& e);
		void RecalculateViewMatrix();
	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		float m_Rotation = 0.0f;
		float m_AspectRatio;
		float m_ZoomLevel;
	};
}
