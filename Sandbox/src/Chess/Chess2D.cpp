#include "Chess2D.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

static bool DoesIntersectTriangle(const glm::vec3& rayOrigin, const glm::vec3& rayVector, const glm::vec3& vertex0, const glm::vec3& vertex1, const glm::vec3& vertex2)
{
	constexpr float epsilon = 0.0000001f;
	glm::vec3 edge1, edge2, h, s, q;
	float a, f, u, v;
	edge1 = vertex1 - vertex0;
	edge2 = vertex2 - vertex0;
	h = glm::cross(edge2, rayVector);
	a = glm::dot(edge1, h);
	if (a > -epsilon && a < epsilon)
		return false;

	f = 1.0f / a;
	s = rayOrigin - vertex0;
	u = f * glm::dot(s, h);
	if (u < 0.0f || u > 1.0f)
		return false;

	q = glm::cross(edge1, s);
	v = f * glm::dot(rayVector, q);
	if (v < 0.0f || u + v > 1.0f)
		return false;
	float t = f * glm::dot(edge2, q);
	if (t > epsilon)
		return true;
	else
		return false;
}

static bool DoesIntersect(const glm::vec3& rayOrigin, const glm::vec3& rayVector, const glm::vec3& quadPos, const glm::vec2& quadSize)
{
	float left = quadPos.x - quadSize.x / 2.0f;
	float top = quadPos.y + quadSize.y / 2.0f;
	float right = quadPos.x + quadSize.x / 2.0f;
	float bottom = quadPos.y - quadSize.y / 2.0f;

	glm::vec3 vertex0 = glm::vec3(left, bottom, quadPos.z);
	glm::vec3 vertex1 = glm::vec3(right, top, quadPos.z);
	glm::vec3 vertex2 = glm::vec3(left, top, quadPos.z);
	glm::vec3 vertex3 = glm::vec3(right, bottom, quadPos.z);

	return DoesIntersectTriangle(rayOrigin, rayVector, vertex0, vertex1, vertex2)
		|| DoesIntersectTriangle(rayOrigin, rayVector, vertex0, vertex3, vertex1);
}

Chess2DLayer::Chess2DLayer()
	: Dominion::Layer("Chess2D") {}

void Chess2DLayer::OnAttach()
{
	/* Setup camera */
	float wHeight = static_cast<float>(Dominion::Application::Get().GetWindow().GetHeight());
	float wWidth = static_cast<float>(Dominion::Application::Get().GetWindow().GetWidth());
	float ratio = wWidth / wHeight;

	m_Camera = Dominion::OrthographicCameraController(ratio, false);
	m_Camera.GetCamera().SetPosition(glm::vec3(0.0f, 0.0f, 0.5f));
}

void Chess2DLayer::OnUpdate(const Dominion::Timestep& timestep)
{
	// Update
	m_Camera.OnUpdate(timestep);
	Dominion::Renderer2D::ResetStats();

	// Render
	Dominion::RenderCommand::SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	Dominion::RenderCommand::Clear();

	Dominion::Renderer2D::BeginScene(m_Camera.GetCamera());

	for (const Square& square : m_Checkerboard.GetSquares())
	{
		Dominion::Renderer2D::DrawQuad({ square.GetOffset().x - 5, square.GetOffset().y - 5 }, { 1.0f, 1.0f }, square.GetColor());
	}

	Dominion::Renderer2D::EndScene();
}

void Chess2DLayer::OnEvent(Dominion::Event& e)
{
	m_Camera.OnEvent(e);
	e.Dispatch<Dominion::KeyPressedEvent>(DM_BIND_EVENT_FN(Chess2DLayer::OnKeyPressed));
}

bool Chess2DLayer::OnKeyPressed(Dominion::KeyPressedEvent& e)
{
	return false;
}
