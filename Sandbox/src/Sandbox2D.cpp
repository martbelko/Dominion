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
	U32 wHeight = static_cast<U32>(wnd.GetHeight());
	U32 wWidth = static_cast<U32>(wnd.GetWidth());
	F32 ratio = static_cast<F32>(wWidth) / wHeight;
	m_Scene.OnViewportResize(wWidth, wHeight);

	m_Camera = m_Scene.CreateEntity("Camera Entity");
	{
		auto& tc = m_Camera.AddComponent<Dominion::TransformComponent>();
		m_Camera.AddComponent<Dominion::CameraComponent>();
	}

	float xOffset = 0.0f;
	for (float y = -3.0f + 0.1f + 0.5f; y < 8.0f; y += 1.05f)
	{
		for (float x = -4.0f + xOffset; x < 4.0f - xOffset; x += 1.05f)
		{
			glm::vec3 pos{ x, y, 0.0f };
			CreateSquareEntity(pos);
		}

		xOffset += 0.5f;
	}

	m_Plane = m_Scene.CreateEntity("Plane");
	{
		auto& tc = m_Plane.AddComponent<Dominion::TransformComponent>();
		tc.scale = glm::vec3(10.0f, 0.1f, 1.0f);
		tc.position.y -= 3.0f;
		auto& sc = m_Plane.AddComponent<Dominion::SpriteRendererComponent>();
		sc.color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		auto& bcc = m_Plane.AddComponent<Dominion::BoxCollider2DComponent>();

		class Script : public Dominion::ScriptableEntity
		{
		private:
			void OnCollisionStart(const Dominion::Collision& collision) override
			{
			}

			void OnCollisionStay(const Dominion::Collision& collision) override
			{
			}

			void OnCollisionEnd(const Dominion::Collision& collision) override
			{
			}
		};

		auto& nsc = m_Plane.AddComponent<Dominion::NativeScriptComponent>();
		nsc.Bind<Script>();
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

Dominion::Entity Sandbox2D::CreateSquareEntity(const glm::vec3 position)
{
	Dominion::Entity square = m_Scene.CreateEntity();
	auto& tc = square.AddComponent<Dominion::TransformComponent>();
	tc.position = position;
	square.AddComponent<Dominion::SpriteRendererComponent>();
	square.AddComponent<Dominion::BoxCollider2DComponent>();
	square.AddComponent<Dominion::RigidBody2DComponent>();

	return square;
}

bool Sandbox2D::OnKeyPressed(Dominion::KeyPressedEvent& e)
{
	return false;
}
