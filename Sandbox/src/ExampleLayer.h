#pragma once

#include "Dominion.h"

class ExampleLayer : public Dominion::Layer
{
public:
	ExampleLayer();
	virtual ~ExampleLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Dominion::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Dominion::Event& e) override;
private:
	Dominion::ShaderLibrary mShaderLibrary;
	Dominion::Ref<Dominion::Shader> mShader;
	Dominion::Ref<Dominion::VertexArray> mVertexArray;

	Dominion::Ref<Dominion::Shader> mFlatColorShader;
	Dominion::Ref<Dominion::VertexArray> mSquareVA;

	Dominion::Ref<Dominion::Texture2D> mTexture, mChernoLogoTexture;

	Dominion::OrthographicCameraController mCameraController;
	glm::vec3 mSquareColor = { 0.2f, 0.3f, 0.8f };
};

