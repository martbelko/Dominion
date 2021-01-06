#include "dmpch.h"
#include "Scene.h"

#include "Dominion/Scene/Components/BaseComponent.h"
#include "Dominion/Scene/Components/TransformComponent.h"
#include "Dominion/Scene/Components/SpriteRendererComponent.h"
#include "Dominion/Scene/Components/CameraComponent.h"
#include "Dominion/Scene/Components/NativeScriptComponent.h"
#include "Dominion/Scene/Components/ColliderComponent.h"

#include "Dominion/Scene/Entity.h"
#include "Dominion/Renderer/Renderer2D.h"
#include "Dominion/Renderer/RenderCommand.h"
#include "Dominion/Scene/ScriptableEntity.h"

#include <glm/glm.hpp>

#include <string_view>

class CollideCallback : public physx::PxSimulationEventCallback
{
	virtual void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count)
	{
	}

	virtual void onWake(physx::PxActor** actors, physx::PxU32 count)
	{
	}

	virtual void onSleep(physx::PxActor** actors, physx::PxU32 count)
	{
	}

	virtual void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs)
	{
		physx::PxRigidActor* shape1 = pairs->shapes[0]->getActor();
		physx::PxTransform t1 = shape1->getGlobalPose();
		physx::PxRigidActor* shape2 = pairs->shapes[1]->getActor();
		physx::PxTransform t2 = shape2->getGlobalPose();

		physx::PxRigidDynamic* dyn = shape1->is<physx::PxRigidDynamic>();
	}

	virtual void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count)
	{
	}

	virtual void onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count)
	{
	}
};

CollideCallback callback;

physx::PxFilterFlags VehicleFilterShader
(physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
	physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
	physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize)
{
	if (physx::PxFilterObjectIsTrigger(attributes0) || physx::PxFilterObjectIsTrigger(attributes1))
	{
		pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT;
		return physx::PxFilterFlag::eDEFAULT;
	}
	// generate contacts for all that were not filtered above
	pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT;

	pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;
	return physx::PxFilterFlag::eDEFAULT;
}

namespace Dominion {

	Scene::Scene(const std::string& sceneName)
		: m_SceneName(sceneName)
	{
		physx::PxSceneDesc sceneDesc(Physics::GetPhysXPhysics()->getTolerancesScale());
		sceneDesc.gravity = physx::PxVec3(0.0f, -0.81f, 0.0f);
		m_PhysicsCPUDispatcher = physx::PxDefaultCpuDispatcherCreate(1);
		sceneDesc.cpuDispatcher = m_PhysicsCPUDispatcher;
		sceneDesc.filterShader = VehicleFilterShader;
		sceneDesc.simulationEventCallback = &callback;
		m_PhysicsScene = Physics::GetPhysXPhysics()->createScene(sceneDesc);

		physx::PxPvdSceneClient* pvdClient = m_PhysicsScene->getScenePvdClient();
		if (pvdClient)
		{
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
		}
	}

	Scene::~Scene()
	{
		if (m_PhysicsScene)
			m_PhysicsScene->release();
		if (m_PhysicsCPUDispatcher)
			m_PhysicsCPUDispatcher->release();
	}

	Entity Scene::CreateEntity()
	{
		return CreateEntity("Entity");
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = Entity(m_Registry.create(), this);
		entity.AddComponent<BaseComponent>(name);
		//entity.AddComponent<TransformComponent>();
		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		entity.Destroy();
	}

	void Scene::OnUpdateEditor(Timestep timestep, const EditorCamera& camera, Entity selectedEntity)
	{
		Renderer2D::BeginScene(camera);

		bool selectedEntityFound = false;
		auto view = m_Registry.view<TransformComponent, SpriteRendererComponent>();
		for (auto entity : view)
		{
			auto [transform, sprite] = view.get<TransformComponent, SpriteRendererComponent>(entity);

			if (static_cast<uint32_t>(entity) == static_cast<uint32_t>(selectedEntity))
			{
				selectedEntityFound = true;
			}
			else
			{
				if (sprite.texture != nullptr)
					Renderer2D::DrawQuad(transform.GetTransform(), sprite.texture, sprite.color, sprite.tilingFactor);
				else
					Renderer2D::DrawQuad(transform.GetTransform(), sprite.color);
			}
		}

		Renderer2D::EndScene();

		if (selectedEntityFound)
		{
			RenderCommand::EnableStencilTest();

			Renderer2D::BeginScene(camera);

			RenderCommand::SetStencilOperation(Dominion::StencilOperation::KEEP, Dominion::StencilOperation::KEEP, Dominion::StencilOperation::REPLACE);
			RenderCommand::SetStencilTestFunc(Dominion::TestFunc::ALWAYS, 0, 0xFF);
			RenderCommand::SetStencilMask(0xFF);

			RenderCommand::ClearStencilBuffer();

			RenderCommand::SetStencilTestFunc(Dominion::TestFunc::ALWAYS, 1, 0xFF);

			TransformComponent& tc = selectedEntity.GetComponent<TransformComponent>();
			SpriteRendererComponent& sprite = selectedEntity.GetComponent<SpriteRendererComponent>();

			if (sprite.texture != nullptr)
				Renderer2D::DrawQuad(tc.GetTransform(), sprite.texture, sprite.color, sprite.tilingFactor);
			else
				Renderer2D::DrawQuad(tc.GetTransform(), sprite.color);

			Renderer2D::Flush();

			RenderCommand::SetStencilTestFunc(Dominion::TestFunc::NOTEQUAL, 1, 0xFF);
			RenderCommand::SetStencilMask(0x00);

			tc.scale += glm::vec3(0.1f, 0.1f, 0.0f);
			Renderer2D::DrawQuad(tc.GetTransform(), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
			tc.scale -= glm::vec3(0.1f, 0.1f, 0.0f);

			Renderer2D::EndScene();

			RenderCommand::DisableStencilTest();
		}
	}

	void Scene::OnUpdateRuntime(Timestep ts)
	{
		// Update scripts
		{
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
			{
				// TODO: Move to Scene::OnScenePlay
				if (!nsc.instance)
				{
					nsc.instance = nsc.InstantiateScript();
					nsc.instance->m_Entity = Entity(entity, this);
					nsc.instance->OnCreate();
				}

				nsc.instance->OnUpdate(ts);
			});
		}

		RenderCommand::ClearColorBuffer();
		RenderCommand::ClearDepthBuffer();
		RenderCommand::ClearStencilBuffer();

		auto view = m_Registry.view<TransformComponent, CameraComponent>();
		for (auto entity : view)
		{
			auto [tc, cc] = view.get<TransformComponent, CameraComponent>(entity);
			if (cc.cameraClearFlag == CameraClearFlag::SOLID_COLOR)
			{
				const glm::vec4& color = cc.backgroundColor;
				RenderCommand::SetClearColor(color.r, color.g, color.b, color.a);
				RenderCommand::ClearColorBuffer();
				RenderCommand::ClearDepthBuffer();
			}
			else if (cc.cameraClearFlag == CameraClearFlag::DEPTH_ONLY)
			{
				RenderCommand::ClearDepthBuffer();
			}

			Renderer2D::BeginScene(cc.camera, tc.GetTransform());

			auto view = m_Registry.view<TransformComponent, SpriteRendererComponent>();
			for (auto entity : view)
			{
				auto [transform, sprite] = view.get<TransformComponent, SpriteRendererComponent>(entity);
				if (sprite.texture != nullptr)
					Renderer2D::DrawQuad(transform.GetTransform(), sprite.texture, sprite.color, sprite.tilingFactor);
				else
					Renderer2D::DrawQuad(transform.GetTransform(), sprite.color);
			}

			Renderer2D::EndScene();
		}
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			CameraComponent& cc = view.get<CameraComponent>(entity);
			SceneCamera& camera = cc.camera;
			camera.SetViewportSize(width, height);
		}
	}

	const std::string& Scene::GetName() const
	{
		return m_SceneName;
	}

	void Scene::SetName(const std::string& sceneName)
	{
		m_SceneName = sceneName;
	}

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(false, "Unknown component");
	}

	template<>
	void Scene::OnComponentAdded<BaseComponent>(Entity entity, BaseComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		component.camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<BoxCollider2DComponent>(Entity entity, BoxCollider2DComponent& component)
	{
		DM_CORE_ASSERT(entity.HasComponent<TransformComponent>(), "Entity must have TransformComponent before adding BoxCollider2DComponent!");
		auto& tc = entity.GetComponent<TransformComponent>();

		physx::PxBoxGeometry squareGeo = physx::PxBoxGeometry(component.scale.x * 0.5f, component.scale.y * 0.5f, 0.0f);
		component.physicsActor = Physics::GetPhysXPhysics()->createRigidDynamic(physx::PxTransform(tc.position.x + component.center.x, tc.position.y + component.center.y, 0.0f));
		//physx::PxShape* squareShape = physx::PxRigidActorExt::createExclusiveShape(*component.physicsActor, squareGeo, *component.physicsMaterial);
		component.physicsActor->setLinearVelocity(physx::PxVec3(0.0f, 0.0f, 0.0f));
		component.physicsActor->setAngularVelocity(physx::PxVec3(0.0f, 0.0f, 0.0f));
		component.physicsActor->userData = reinterpret_cast<void*>(static_cast<U32>(entity));
		m_PhysicsScene->addActor(*component.physicsActor);
	}

}
