#include "Sandbox2D.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

bool DoesIntersectTriangle(const glm::vec3& rayOrigin, const glm::vec3& rayVector, const glm::vec3& vertex0, const glm::vec3& vertex1, const glm::vec3& vertex2)
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

bool DoesIntersect(const glm::vec3& rayOrigin, const glm::vec3& rayVector, const glm::vec3& quadPos, const glm::vec2& quadSize)
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
	/* Load resources */
	m_Texture2D = Dominion::Texture2D::Create("assets/Textures/TestTexture.jpg");
	m_TestTexture = Dominion::Texture2D::Create("assets/Textures/unnamed.png");

	/* Disable cursor */
	Dominion::Application::Get().GetWindow().ShowCursor(false);

	/* Setup camera */
	float wHeight = static_cast<float>(Dominion::Application::Get().GetWindow().GetHeight());
	float wWidth = static_cast<float>(Dominion::Application::Get().GetWindow().GetWidth());
	float ratio = wWidth / wHeight;

	m_Camera = Dominion::OrthographicCameraController(ratio, false);
	m_Camera.GetCamera().SetPosition(glm::vec3(0.0f, 0.0f, 0.5f));

	Dominion::Timer timer;
	timer.Start();
	for (int i = 0; i < 100000; ++i)
	{
		Dominion::Random::RandomInt8(-128, 127);
		Dominion::Random::RandomInt16(-2000, 2500);
		Dominion::Random::RandomInt32(-65453125, 4552421);
		Dominion::Random::RandomInt64(24515545151541, 54415154541651);
		Dominion::Random::RandomUint8(0, 250);
		Dominion::Random::RandomUint16(0, 151020);
		Dominion::Random::RandomUint32(0, 1541561);
		Dominion::Random::RandomUint64(645, 32552465);
		Dominion::Random::RandomFloat(-50.0f, 50.0f);
		Dominion::Random::RandomDouble(-50.0f, 50.0f);
	}

	float timeElapsed = timer.PeekNano();
	DM_INFO("1 000 000 random numbers were generated in {0} nanoseconds", timeElapsed);
}

void Sandbox2D::OnUpdate(const Dominion::Timestep& timestep)
{
	static int sign = -1;
	m_Rotation += timestep * sign * m_RotationSpeed;
	m_Rotation = std::clamp(m_Rotation, -180.0f, 180.0f);
	if (m_Rotation == -180.0f)
		sign = 1;
	else if (m_Rotation == 180.0f)
		sign = -1;

	// Update
	m_Camera.OnUpdate(timestep);
	Dominion::Renderer2D::ResetStats();

	Dominion::RenderCommand::SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	Dominion::RenderCommand::ClearColorBuffer();

	Dominion::Renderer2D::BeginScene(m_Camera.GetCamera());

	float cap = m_Count / 2.0f;
	for (float y = -cap; y < cap; y += 0.5f)
	{
		for (float x = -cap; x < cap; x += 0.5f)
		{
			glm::vec4 color = { (x + cap) / static_cast<float>(m_Count),
				0.3f,
				(y + cap) / static_cast<float>(m_Count),
				1.0f };

			bool intersect = DoesIntersect(m_Camera.GetCamera().GetPosition(), { 0.0f, 0.0f, -1.0f }, { x, y, 0.0f }, { 0.45f, 0.45f });
			if (intersect)
				Dominion::Renderer2D::DrawQuad({ x, y, }, { 0.45f, 0.45f }, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
			else
				Dominion::Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
		}
	}

	Dominion::Renderer2D::DrawQuad(glm::mat4(1.0f), m_Texture2D);
	Dominion::Renderer2D::DrawQuad(glm::mat4(1.0f), m_TestTexture, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 5.0f);

	Dominion::Renderer2D::EndScene();
}

void Sandbox2D::OnEvent(Dominion::Event& e)
{
	m_Camera.OnEvent(e);
	e.Dispatch<Dominion::KeyPressedEvent>(DM_BIND_EVENT_FN(Sandbox2D::OnKeyPressed));
}

bool Sandbox2D::OnKeyPressed(Dominion::KeyPressedEvent& e)
{
	if (e.GetKeyCode() == Dominion::Key::Escape)
		m_ShowCursor = !m_ShowCursor;

	return false;
}
