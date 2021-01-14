#include "dmpch.h"
#include "Scene.h"

#include "Dominion/Scene/Entity.h"
#include "Dominion/Scene/ScriptableEntity.h"

#include "Dominion/Scene/Components/BaseComponent.h"
#include "Dominion/Scene/Components/TransformComponent.h"
#include "Dominion/Scene/Components/SpriteRendererComponent.h"
#include "Dominion/Scene/Components/CameraComponent.h"
#include "Dominion/Scene/Components/NativeScriptComponent.h"
#include "Dominion/Scene/Components/ColliderComponent.h"
#include "Dominion/Scene/Components/RigidBodyComponent.h"

#include "Dominion/Renderer/Renderer2D.h"
#include "Dominion/Renderer/RenderCommand.h"

#include "Dominion/Physics/Collision.h"

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <string_view>

physx::PxFilterFlags DefaultFilterShader
(physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
	physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
	physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize)
{
	if (physx::PxFilterObjectIsTrigger(attributes0) || physx::PxFilterObjectIsTrigger(attributes1))
	{
		pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT;
		return physx::PxFilterFlag::eDEFAULT;
	}

	pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT;
	pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;
	pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS;
	pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_LOST;

	return physx::PxFilterFlag::eDEFAULT;
}

namespace Dominion {

	Scene::Scene(const std::string& sceneName)
		: m_SceneName(sceneName)
	{
		physx::PxSceneDesc sceneDesc(Physics::GetPhysXPhysics()->getTolerancesScale());
		sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
		m_PhysicsCPUDispatcher = physx::PxDefaultCpuDispatcherCreate(1);
		sceneDesc.cpuDispatcher = m_PhysicsCPUDispatcher;
		sceneDesc.filterShader = DefaultFilterShader;
		sceneDesc.simulationEventCallback = &m_PhysicsSimulationEventCallback;
		sceneDesc.userData = this;
		m_PhysicsScene = Physics::GetPhysXPhysics()->createScene(sceneDesc);

		// TODO: Remove PVD from physics entirely
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
		// Destroy scripts
		{
			m_Registry.view<Dominion::NativeScriptComponent>().each([](auto& nsc)
			{
				nsc.Destroy();
			});
		}

		if (m_PhysicsScene)
			m_PhysicsScene->release();
		if (m_PhysicsCPUDispatcher)
			m_PhysicsCPUDispatcher->release();
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = Entity(m_Registry.create(), this);
		entity.AddComponent<BaseComponent>(name);
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

		// Update physics
		{
			static float mAccumulator = 0.0f;
			mAccumulator += ts;
			if (mAccumulator >= 1 / 60.0f)
			{
				mAccumulator -= 1 / 60.0f;
				auto view = m_Registry.view<TransformComponent, BoxCollider2DComponent>();
				for (entt::entity entity : view)
				{
					auto& tc = view.get<TransformComponent>(entity);
					auto& bcc = view.get<BoxCollider2DComponent>(entity);
					physx::PxTransform physxTransform = bcc.physicsActor->getGlobalPose();
					glm::vec3& position = *reinterpret_cast<glm::vec3*>(&physxTransform.p);

					glm::vec3 vec3CenterOffset = glm::vec3(bcc.centerOffset, 0.0f);
					if (position != tc.position + vec3CenterOffset)
					{
						position = tc.position + vec3CenterOffset; // This is directly changing physxTransform variable (it's position)
						bcc.physicsActor->setGlobalPose(physxTransform);
					}
				}

				m_Registry.view<RigidBody2DComponent>().each([=](entt::entity entity, auto& rbc)
				{
					BoxCollider2DComponent& bcc = m_Registry.get<BoxCollider2DComponent>(entity);
					physx::PxRigidDynamic* dyn = reinterpret_cast<physx::PxRigidDynamic*>(bcc.physicsActor);
					float physxMass = dyn->getMass();
					if (physxMass != rbc.mass)
					{
						dyn->setMass(rbc.mass);
					}
				});

				m_PhysicsScene->collide(1 / 60.0f);
				m_PhysicsScene->fetchCollision(true);
				m_PhysicsScene->advance();
				m_PhysicsScene->fetchResults(true);

				for (const InternalCollision& internalCollision : m_PhysicsSimulationEventCallback.collisions)
				{
					Collision collision;
					collision.entity1 = Entity(internalCollision.entity1Index, internalCollision.scene);
					collision.entity2 = Entity(internalCollision.entity2Index, internalCollision.scene);
					if (collision.entity1.HasComponent<NativeScriptComponent>())
					{
						auto& nsc = collision.entity1.GetComponent<NativeScriptComponent>();
						if (internalCollision.flag == InternalCollision::Flag::COLLISION_START)
							nsc.instance->OnCollisionStart(collision);
						if (internalCollision.flag == InternalCollision::Flag::COLLISION_STAY)
							nsc.instance->OnCollisionStay(collision);
						if (internalCollision.flag == InternalCollision::Flag::COLLISION_END)
							nsc.instance->OnCollisionEnd(collision);
					}
					if (collision.entity2.HasComponent<NativeScriptComponent>())
					{
						auto& nsc = collision.entity2.GetComponent<NativeScriptComponent>();
						if (internalCollision.flag == InternalCollision::Flag::COLLISION_START)
							nsc.instance->OnCollisionStart(collision);
						if (internalCollision.flag == InternalCollision::Flag::COLLISION_STAY)
							nsc.instance->OnCollisionStay(collision);
						if (internalCollision.flag == InternalCollision::Flag::COLLISION_END)
							nsc.instance->OnCollisionEnd(collision);
					}
				}

				m_PhysicsSimulationEventCallback.collisions.clear();

				U32 nActors = m_PhysicsScene->getNbActors(physx::PxActorTypeFlag::eRIGID_DYNAMIC);
				if (nActors)
				{
					std::vector<physx::PxRigidActor*> actors(nActors);
					m_PhysicsScene->getActors(physx::PxActorTypeFlag::eRIGID_DYNAMIC, reinterpret_cast<physx::PxActor**>(&actors[0]), nActors);
					for (physx::PxRigidActor* dyn : actors)
					{
						m_PhysicsScene->lockRead();
						physx::PxTransform t = dyn->getGlobalPose();
						m_PhysicsScene->unlockRead();
						glm::vec3 position = { t.p.x, t.p.y, t.p.z };
						glm::quat rotation = glm::quat(t.q.w, t.q.x, t.q.y, t.q.z);
						glm::vec3 angle = glm::eulerAngles(rotation);

						U32 entityID = static_cast<U32>(reinterpret_cast<U64>(dyn->userData));
						Entity entity(entityID, this);
						auto& tc = entity.GetComponent<TransformComponent>();
						auto& bcc = entity.GetComponent<BoxCollider2DComponent>();
						tc.position = position - glm::vec3(bcc.centerOffset, 0.0f);
						tc.rotation = angle;
					}
				}
			}
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

		physx::PxBoxGeometry squareGeo = physx::PxBoxGeometry(tc.scale.x * 0.5f, tc.scale.y * 0.5f, 1.0f);
		component.physicsActor = Physics::GetPhysXPhysics()->createRigidStatic(physx::PxTransform(tc.position.x + component.centerOffset.x, tc.position.y + component.centerOffset.y, 0.0f));
		physx::PxShape* squareShape = physx::PxRigidActorExt::createExclusiveShape(*component.physicsActor, squareGeo, *component.physicsMaterial);
		component.physicsActor->userData = reinterpret_cast<void*>(static_cast<U64>(entity.GetID()));
		m_PhysicsScene->addActor(*component.physicsActor);
	}

	template<>
	void Scene::OnComponentAdded<RigidBody2DComponent>(Entity entity, RigidBody2DComponent& component)
	{
		DM_CORE_ASSERT(entity.HasComponent<BoxCollider2DComponent>(), "Entity must have ColliderComponent before adding RigidBody2DComponent!");

		auto& tc = entity.GetComponent<TransformComponent>();
		auto& bcc = entity.GetComponent<BoxCollider2DComponent>();
		bcc.physicsActor->release();

		physx::PxBoxGeometry squareGeo = physx::PxBoxGeometry(1.0f * 0.5f, 1.0f * 0.5f, 1.0f);
		physx::PxRigidDynamic* dyn = Physics::GetPhysXPhysics()->createRigidDynamic(physx::PxTransform(tc.position.x + bcc.centerOffset.x, tc.position.y + bcc.centerOffset.y, 0.0f));;
		physx::PxShape* squareShape = physx::PxRigidActorExt::createExclusiveShape(*dyn, squareGeo, *bcc.physicsMaterial);
		dyn->userData = reinterpret_cast<void*>(static_cast<U64>(entity.GetID()));
		dyn->setRigidDynamicLockFlags(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Z | physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X | physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y); // Locks only to 2D
		dyn->setMass(component.mass);
		m_PhysicsScene->addActor(*dyn);

		bcc.physicsActor = dyn;
		dyn->wakeUp();
	}

	void Scene::CollideCallback::onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count)
	{

	}

	void Scene::CollideCallback::onWake(physx::PxActor** actors, physx::PxU32 count)
	{

	}

	void Scene::CollideCallback::onSleep(physx::PxActor** actors, physx::PxU32 count)
	{

	}

	void Scene::CollideCallback::onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs)
	{
		if (nbPairs != 1)
			DM_ASSERT(false, "More than one pair detected in collision!");

		Dominion::Scene* scene = static_cast<Dominion::Scene*>(pairHeader.actors[0]->getScene()->userData);
		U32 ent1Index = static_cast<U32>(reinterpret_cast<U64>(pairHeader.actors[0]->userData));
		U32 ent2Index = static_cast<U32>(reinterpret_cast<U64>(pairHeader.actors[1]->userData));

		InternalCollision collision;
		collision.entity1Index = ent1Index;
		collision.entity2Index = ent2Index;
		collision.scene = scene;

		const physx::PxContactPair& cp = *pairs;
		if (pairs->events & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			collision.flag = InternalCollision::Flag::COLLISION_START;
		}
		else if (pairs->events & physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
		{
			collision.flag = InternalCollision::Flag::COLLISION_STAY;
		}
		else if (pairs->events & physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			collision.flag = InternalCollision::Flag::COLLISION_END;
		}

		collisions.emplace_back(collision);
	}

	void Scene::CollideCallback::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count)
	{

	}

	void Scene::CollideCallback::onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count)
	{

	}

}
