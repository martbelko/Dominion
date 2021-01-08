#include "dmpch.h"
#include "Application.h"

#include "Dominion/Core/Input.h"
#include "Dominion/Renderer/RenderCommand.h"
#include "Dominion/Renderer/Buffer.h"
#include "Dominion/Renderer/InputLayout.h"
#include "Dominion/Renderer/Renderer.h"

#include "Dominion/Core/Physics.h"

/* TODO: Temporary */
#include "Dominion/Scene/Scene.h"
#include "Dominion/Scene/Entity.h"
#include "Dominion/Scene/Components/BaseComponent.h"
#include "Dominion/Scene/Components/TransformComponent.h"
#include "Dominion/Scene/Components/SpriteRendererComponent.h"
#include "Dominion/Scene/Components/CameraComponent.h"
#include "Dominion/Scene/Components/NativeScriptComponent.h"
#include "Dominion/Scene/Components/ColliderComponent.h"
#include "Dominion/Scene/Components/RigidBodyComponent.h"

#include "Dominion/Renderer/PerspectiveCameraController.h"
#include "Dominion/Renderer/Model.h"
#include "Dominion/Renderer/Shader.h"
#include "Dominion/Events/KeyEvent.h"
#include "Dominion/Utils/Random.h"

#if DM_INCLUDE_IMGUI
	#include "DominionImGui.h"
#endif

//#include <PxPhysicsAPI.h>

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

using namespace Dominion;

Scene* gScene;

PerspectiveCameraController* m_CameraController;
Ref<Pipeline> pipeline;
Ref<Shader> shader;
Ref<Pipeline> planePipeline;

void initPhysics(bool interactive)
{
	gScene = new Scene();
	U32 wwidth = Application::Get().GetWindow().GetWidth(), wheight = Application::Get().GetWindow().GetHeight();
	gScene->OnViewportResize(wwidth, wheight);
	gScene->GetPhysicsScene()->userData = gScene;

	Entity camera = gScene->CreateEntity();
	{
		auto& tc = camera.AddComponent<TransformComponent>();
		tc.position = tc.position + glm::vec3(0.0f, 0.0f, 0.5f);
		auto& cc = camera.AddComponent<CameraComponent>();
	}

	Entity square = gScene->CreateEntity();
	{
		square.AddComponent<TransformComponent>();
		square.AddComponent<SpriteRendererComponent>();
		square.AddComponent<BoxCollider2DComponent>();
		square.AddComponent<RigidBody2DComponent>();
	}

	Entity plane = gScene->CreateEntity("Plane");
	{
		auto& tc = plane.AddComponent<TransformComponent>();
		tc.position.y -= 3.0f;
		auto& sc = plane.AddComponent<SpriteRendererComponent>();
		sc.color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		auto& bcc = plane.AddComponent<BoxCollider2DComponent>();
	}

	/*PxMaterial* mat = Physics::GetPhysXPhysics()->createMaterial(0.5f, 0.5f, 0.1f);
	PxBoxGeometry squareGeo = PxBoxGeometry(0.5f, 0.5f, 0.0f);
	PxRigidDynamic* squareActor = Physics::GetPhysXPhysics()->createRigidDynamic(PxTransform(0.0f, 0.0f, 0.0f));
	PxShape* squareShape = PxRigidActorExt::createExclusiveShape(*squareActor, squareGeo, *mat);
	squareActor->setLinearVelocity(PxVec3(0, 0, 0));
	squareActor->setAngularVelocity(PxVec3(0, 0, 0));
	squareActor->userData = reinterpret_cast<void*>(static_cast<U32>(square));
	gScene->GetPhysicsScene()->addActor(*squareActor);*/

	/*PxMaterial* woodMaterial = Physics::GetPhysXPhysics()->createMaterial(0.5f, 0.5f, 0.1f);
	PxMaterial* concreteMaterial = Physics::GetPhysXPhysics()->createMaterial(0.8f, 0.8f, 0.55f);

	PxBoxGeometry aBoxGeometry = PxBoxGeometry(1.0f / 2, 1.0f / 2, 1.0f / 2);

	F32 angleX = 0.0f;
	F32 angleY = 0.0f;
	F32 angleZ = 0.0f;

	glm::quat q = glm::quat(glm::vec3(angleX, angleY, angleZ));

	for (float i = 1.0f; i < 10.5f; i += 1.0f)
	{
		for (float j = 0.0f; j < 4.0f; j += 1.0f)
		{
			for (float k = 0.0f; k < 4.0f; k += 1.0f)
			{
				PxRigidDynamic* aBoxActor = Physics::GetPhysXPhysics()->createRigidDynamic(PxTransform(j, i - 0.5f, k, PxQuat(q.x, q.y, q.z, q.w)));
				PxShape* aBoxShape = PxRigidActorExt::createExclusiveShape(*aBoxActor, aBoxGeometry, *woodMaterial);
				aBoxActor->setLinearVelocity(PxVec3(0, 0, 0));
				aBoxActor->setAngularVelocity(PxVec3(0, 0, 0));
				gScene->GetPhysicsScene()->addActor(*aBoxActor);
			}
		}
	}

	PxRigidStatic* groundPlaneActor = PxCreatePlane(*Physics::GetPhysXPhysics(), PxPlane(0, 1, 0, 0), *concreteMaterial);
	gScene->GetPhysicsScene()->addActor(*groundPlaneActor);

	float vertices[] = {
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f
	};

	U32 indices[] = {
		0, 1, 2,
		2, 3, 0,
		1, 5, 6,
		6, 2, 1,
		7, 6, 5,
		5, 4, 7,
		4, 0, 3,
		3, 7, 4,
		4, 5, 1,
		1, 0, 4,
		3, 2, 6,
		6, 7, 3
	};

	auto vb = VertexBuffer::Create(vertices, sizeof(vertices));
	auto ib = IndexBuffer::Create(indices, sizeof(indices) / sizeof(U32));
	auto inputLayout = InputLayout::Create({
				{ "Position", DataType::Float3 }
	});

	pipeline = Pipeline::Create(vb, ib, inputLayout);
	shader = Shader::Create("ModelShader", "assets/Shaders/CubeShaderVS.glsl", "assets/Shaders/CubeShaderPS.glsl");

	float planevertices[] = {
		0.0f, -10.0f, -10.0f,
		0.0f, -10.0f,  10.0f,
		0.0f,  10.0f,  10.0f,
		0.0f,  10.0f, -10.0f
	};

	U32 planeindices[] = {
		0, 1, 2,
		2, 3, 0
	};

	auto planevb = VertexBuffer::Create(planevertices, sizeof(planevertices));
	auto planeib = IndexBuffer::Create(planeindices, sizeof(planeindices) / sizeof(U32));
	planePipeline = Pipeline::Create(planevb, planeib, inputLayout);

	m_CameraController = new PerspectiveCameraController(1280.0f / 720.0f);
	m_CameraController->GetCamera().SetPosition({ 1, 1, 5 });*/
}

void stepPhysics(float ts)
{
	gScene->GetPhysicsScene()->simulate(ts);
	gScene->GetPhysicsScene()->fetchResults(true);
}

void cleanupPhysics(bool)
{
	delete gScene;
}

void RenderLoop(Timestep ts)
{
	/*stepPhysics(ts.GetSeconds());

	RenderCommand::ClearColorBuffer();
	RenderCommand::ClearDepthBuffer();
	Renderer::BeginScene(m_CameraController->GetCamera());

	PxScene* scene;
	PxGetPhysics().getScenes(&scene, 1);
	PxU32 nbActors = scene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC);
	if (nbActors)
	{
		std::vector<PxRigidActor*> actors(nbActors - 1);
		scene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC, reinterpret_cast<PxActor**>(&actors[0]), nbActors);
		for (PxRigidActor* dyn : actors)
		{
			gScene->GetPhysicsScene()->lockRead();
			PxTransform t = dyn->getGlobalPose();
			gScene->GetPhysicsScene()->unlockRead();
			glm::vec3 pos = { t.p.x, t.p.y, t.p.z };
			glm::quat rot = glm::quat(t.q.w, t.q.x, t.q.y, t.q.z);
			glm::mat4 locTrans = glm::translate(glm::mat4(1.0f), pos) * glm::toMat4(rot) * glm::scale(glm::mat4(1.0f), { 0.5f, 0.5f, 0.5f });

			// Cube
			shader->Bind();
			shader->SetFloat4("u_Color", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
			Renderer::Submit(shader, pipeline, locTrans);
		}

		std::vector<PxRigidActor*> staticActors(1);
		scene->getActors(PxActorTypeFlag::eRIGID_STATIC, reinterpret_cast<PxActor**>(&staticActors[0]), nbActors);
		PxRigidStatic* rigidStatic = static_cast<PxRigidStatic*>(staticActors[0]);
		gScene->GetPhysicsScene()->lockRead();
		PxTransform t = rigidStatic->getGlobalPose();
		gScene->GetPhysicsScene()->unlockRead();
		glm::vec3 pos = { t.p.x, t.p.y, t.p.z };
		glm::quat rot = glm::quat(t.q.w, t.q.x, t.q.y, t.q.z);
		glm::mat4 test = glm::toMat4(rot);
		glm::mat4 locTrans = glm::translate(glm::mat4(1.0f), pos) * glm::toMat4(rot);

		shader->SetFloat4("u_Color", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		Renderer::Submit(shader, planePipeline, locTrans);
	}

	Renderer::EndScene();*/
}

namespace Dominion {

	Application* Application::s_Application = nullptr;

	Application::Application(const std::string_view& name)
	{
		DM_PROFILE_FUNCTION();

		if (s_Application == nullptr)
		{
			s_Application = this;
			WindowProps wndProps;
			wndProps.Title = name;
			wndProps.Width = 1280;
			wndProps.Height = 720;
			m_Window = Window::Create(DM_BIND_EVENT_FN(Application::OnEvent), wndProps);

			Renderer::Init();

#if DM_INCLUDE_IMGUI == 1
			m_ImGuiLayer = new ImGuiLayer();
			PushOverlay(m_ImGuiLayer);
#endif
			initPhysics(true);
			m_Window->ShowCursor(false);
		}
		else
		{
			DM_DEBUGBREAK();
		}
	}

	Application::~Application()
	{
		DM_PROFILE_FUNCTION();

#if DM_INCLUDE_IMGUI == 1
		m_LayerStack.PopOverlay(m_ImGuiLayer);
		delete m_ImGuiLayer;
#endif
		Renderer::Shutdown();
		delete m_Window;
		cleanupPhysics(false);
	}

	void Application::Run()
	{
		DM_PROFILE_FUNCTION();

		m_LastFrameTime = std::chrono::system_clock::now();
		while (m_Running)
		{
			DM_PROFILE_SCOPE("Run Loop");

			auto end = std::chrono::system_clock::now();
			Timestep ts = static_cast<F32>(std::chrono::duration_cast<std::chrono::nanoseconds>(end - m_LastFrameTime).count());
			m_LastFrameTime = std::chrono::system_clock::now();

			//m_CameraController->OnUpdate(ts);

			/*if (Input::IsKeyPressed(Key::Space))
			{
				PxScene* scene;
				PxGetPhysics().getScenes(&scene, 1);
				PxU32 nbActors = scene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC);
				if (nbActors)
				{
					std::vector<PxRigidActor*> actors(nbActors);
					scene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC, reinterpret_cast<PxActor**>(&actors[0]), nbActors);
					PxRigidDynamic* rigidDynamic = static_cast<PxRigidDynamic*>(actors[0]);
					gScene->GetPhysicsScene()->lockWrite();
					rigidDynamic->addForce(PxVec3(0, 5, 0));
					gScene->GetPhysicsScene()->unlockWrite();
				}
			}*/

			/* TODO: Temporary for physics test purposes */
			gScene->OnUpdateRuntime(ts);

			if (!m_Minimized)
			{
				{
					DM_PROFILE_SCOPE("LayerStack OnUpdate");

					/*for (Layer* layer : m_LayerStack)
						layer->OnUpdate(ts);*/
				}
			}

			{
				DM_PROFILE_SCOPE("LayerStack OnImGuiRenderr");

#if DM_INCLUDE_IMGUI == 1
				m_ImGuiLayer->Begin();
				for (Layer* layer : m_LayerStack)
					layer->OnImGuiRender();
				m_ImGuiLayer->End();
#endif
			}

			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		DM_PROFILE_FUNCTION();

		e.Dispatch<WindowCreatedEvent>(DM_BIND_EVENT_FN(Application::OnWindowCreated));
		e.Dispatch<WindowClosedEvent>(DM_BIND_EVENT_FN(Application::OnWindowClosed));
		e.Dispatch<WindowResizedEvent>(DM_BIND_EVENT_FN(Application::OnWindowResized));
		/*e.Dispatch<KeyPressedEvent>(DM_BIND_EVENT_FN(Application::OnKeyPressed));
		e.Dispatch<MousePressedEvent>(DM_BIND_EVENT_FN(Application::OnMousePressed));

		m_CameraController->OnEvent(e);*/

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			if (e.Handled)
				break;
			(*it)->OnEvent(e);
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		DM_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		DM_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	ImGuiLayer* Application::GetImGuiLayer()
	{
		return m_ImGuiLayer;
	}

	Window& Application::GetWindow() const
	{
		return *m_Window;
	}

	void Application::Close()
	{
		m_Running = false;
	}

	bool Application::OnWindowCreated(WindowCreatedEvent& event)
	{
		return false;
	}

	bool Application::OnWindowClosed(WindowClosedEvent& event)
	{
		m_Window->Close();
		m_Running = false;
		return false;
	}

	bool Application::OnWindowResized(WindowResizedEvent& e)
	{
		DM_PROFILE_FUNCTION();

		uint32_t width = e.GetWidth();
		uint32_t height = e.GetHeight();
		if (width == 0 || height == 0)
		{
			m_Minimized = true;
			return false;
		}

		Renderer::OnWindowResize(width, height);
		gScene->OnViewportResize(width, height);

		m_Minimized = false;
		return false;
	}

	bool Application::OnKeyPressed(KeyPressedEvent& e)
	{
		/*if (e.GetKeyCode() == Key::R)
		{
			PxScene* scene;
			PxGetPhysics().getScenes(&scene, 1);
			PxU32 nbActors = scene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC);
			if (nbActors)
			{
				for (float i = 1.0f; i < 10.5f; i += 1.0f)
				{
					for (float j = 0.0f; j < 4.0f; j += 1.0f)
					{
						for (float k = 0.0f; k < 4.0f; k += 1.0f)
						{
							PxRigidDynamic* aBoxActor = Physics::GetPhysXPhysics()->createRigidDynamic(PxTransform(j, i - 0.5f, k, PxQuat(q.x, q.y, q.z, q.w)));
							PxShape* aBoxShape = PxRigidActorExt::createExclusiveShape(*aBoxActor, aBoxGeometry, *woodMaterial);
							aBoxActor->setLinearVelocity(PxVec3(0, 0, 0));
							aBoxActor->setAngularVelocity(PxVec3(0, 0, 0));
							gScene->addActor(*aBoxActor);
						}
					}
				}
			}
		}*/

		return false;
	}

	bool Application::OnMousePressed(MousePressedEvent& e)
	{
		/*if (e.GetButton() == Mouse::Button0)
		{
			PxMaterial* woodMaterial = Physics::GetPhysXPhysics()->createMaterial(0.5f, 0.5f, 0.1f);
			PxBoxGeometry aBoxGeometry = PxBoxGeometry(1.0f / 2, 1.0f / 2, 1.0f / 2);

			const PerspectiveCamera& camera = m_CameraController->GetCamera();
			const glm::vec3& pos = camera.GetPosition();
			glm::vec3 ray = camera.CreateRay() * 50.0f;
			glm::vec3 cubePos = pos + camera.CreateRay();

			PxRigidDynamic* aBoxActor = Physics::GetPhysXPhysics()->createRigidDynamic(PxTransform(cubePos.x, cubePos.y + 1.0f, cubePos.z + 1.0f));
			PxShape* aBoxShape = PxRigidActorExt::createExclusiveShape(*aBoxActor, aBoxGeometry, *woodMaterial);
			aBoxActor->setLinearVelocity(PxVec3(ray.x, ray.y, ray.z));
			PxRigidBodyExt::updateMassAndInertia(*aBoxActor, 1000.0f);
			gScene->GetPhysicsScene()->addActor(*aBoxActor);
		}*/

		return false;
	}

	Application& Application::Get()
	{
		return *s_Application;
	}

}
