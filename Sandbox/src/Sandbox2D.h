#pragma once

#include "Dominion.h"

class Sandbox2D : public Dominion::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Dominion::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Dominion::Event& e) override;
private:
	Dominion::OrthographicCameraController mCameraController;

	// Temp
	Dominion::Ref<Dominion::VertexArray> mSquareVA;
	Dominion::Ref<Dominion::Shader> mFlatColorShader;

	Dominion::Ref<Dominion::Texture2D> mCheckerboardTexture;

	glm::vec4 mSquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};
