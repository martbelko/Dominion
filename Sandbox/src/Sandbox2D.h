#pragma once

#include <Dominion.h>

class Sandbox2D : public Dominion::Layer
{
public:
	Sandbox2D();
	void OnUpdate(const Dominion::Timestep& timestep) override;
	void OnEvent(Dominion::Event& e) override;
	void OnImGuiRender() override;
private:
	Dominion::ShaderLibrary m_ShaderLibrary;
	Dominion::Ref<Dominion::Pipeline> m_Pipeline;

	Dominion::Ref<Dominion::Texture2D> m_Texture2D;
	Dominion::Ref<Dominion::Texture2D> m_TestTexture;

	Dominion::OrthographicCamera m_Camera;
};
