#pragma once

#include <Dominion.h>

namespace Dominion {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		void OnUpdate(const Timestep& timestep) override;
		void OnEvent(Event& e) override;
		void OnImGuiRender() override;
	private:
		Ref<Framebuffer> m_Framebuffer;

		Ref<Texture2D> m_Texture2D;
		Ref<Texture2D> m_TestTexture;

		OrthographicCamera m_Camera;

		float m_Rotation = 0.0f;
		float m_RotationSpeed = 40.0f;

		int m_Count = 10;
	};

}
