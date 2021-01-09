#include "Sandbox2D.h"

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

Sandbox2D::Sandbox2D()
	: Dominion::Layer("Sandbox2D") {}

void Sandbox2D::OnAttach()
{
	const Dominion::Window& wnd = Dominion::Application::Get().GetWindow();
	F32 wHeight = static_cast<F32>(wnd.GetHeight());
	F32 wWidth = static_cast<F32>(wnd.GetWidth());
	F32 ratio = wWidth / wHeight;
	m_Scene.OnViewportResize(wWidth, wHeight);

	m_Square = m_Scene.CreateEntity();
	{
		m_Square.AddComponent<Dominion::TransformComponent>();
		auto& sprite = m_Square.AddComponent<Dominion::SpriteRendererComponent>();
		sprite.texture = Dominion::Texture2D::Create("assets/Textures/TestTexture.jpg");
		m_Square.AddComponent<Dominion::BoxCollider2DComponent>();
		m_Square.AddComponent<Dominion::RigidBody2DComponent>();
	}

	m_Camera = m_Scene.CreateEntity("Camera Entity");
	{
		m_Camera.AddComponent<Dominion::TransformComponent>();
		m_Camera.AddComponent<Dominion::CameraComponent>();
	}

	m_Plane = m_Scene.CreateEntity("Plane");
	{
		auto& tc = m_Plane.AddComponent<Dominion::TransformComponent>();
		tc.position.y -= 3.0f;
		auto& sc = m_Plane.AddComponent<Dominion::SpriteRendererComponent>();
		sc.color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		auto& bcc = m_Plane.AddComponent<Dominion::BoxCollider2DComponent>();
	}
}

void Sandbox2D::OnUpdate(const Dominion::Timestep& timestep)
{
	m_Scene.OnUpdateRuntime(timestep);
}

void Sandbox2D::OnEvent(Dominion::Event& e)
{
	e.Dispatch<Dominion::KeyPressedEvent>(DM_BIND_EVENT_FN(Sandbox2D::OnKeyPressed));
}

bool Sandbox2D::OnKeyPressed(Dominion::KeyPressedEvent& e)
{
	return false;
}
