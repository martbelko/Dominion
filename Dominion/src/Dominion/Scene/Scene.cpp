#include "dmpch.h"
#include "Scene.h"

#include "Components.h"
#include "Entity.h"
#include "SceneCamera.h"
#include "ScriptableEntity.h"

#include "Dominion/Renderer/Renderer2D.h"
#include "Dominion/Core/Input.h"

#include <glm/glm.hpp>

#include "box2D/b2_world.h"
#include "box2D/b2_body.h"
#include "box2D/b2_fixture.h"
#include "box2D/b2_polygon_shape.h"
#include "box2D/b2_circle_shape.h"

namespace Dominion {

	FontRenderer* mFontRenderer = nullptr;

	static b2BodyType DominionRigidbody2DTypeToBox2DBodyType(Rigidbody2DComponent::BodyType bodyType)
	{
		switch (bodyType)
		{
			case Rigidbody2DComponent::BodyType::Static: return b2_staticBody;
			case Rigidbody2DComponent::BodyType::Dynamic: return b2_dynamicBody;
			case Rigidbody2DComponent::BodyType::Kinematic: return b2_kinematicBody;
		}

		DM_CORE_ASSERT(false, "Unknown Rigidbody2DType");
		return b2_staticBody;
	}

	Scene::Scene()
	{
		if (mFontRenderer == nullptr)
		{
			mFontRenderer = new FontRenderer();
			mFontRenderer->LoadFont("assets/fonts/consolas/CONSOLA.ttf");
		}
	}

	template<typename Component>
	static void CopyComponentIfExists(entt::registry& dst, entt::entity dstEntity, entt::registry& src, entt::entity srcEntity)
	{
		if (src.has<Component>(srcEntity))
			dst.emplace_or_replace<Component>(dstEntity, src.get<Component>(srcEntity));
	}

	template<typename Component>
	static void CopyComponentIfExists(Entity dst, Entity src)
	{
		if (src.HasComponent<Component>())
			dst.AddOrReplaceComponent<Component>(src.GetComponent<Component>());
	}

	Scene::Scene(Scene& other)
	{
		mViewportWidth = other.mViewportWidth;
		mViewportHeight = other.mViewportHeight;

		auto idView = other.mRegistry.view<IDComponent>();
		for (auto eid : idView)
		{
			Entity entity = Entity(eid, &other);

			UUID uuid = entity.GetUUID();
			const auto& name = entity.GetComponent<TagComponent>().tag;

			Entity newEntity = CreateEntity(uuid, name);
			CopyComponentIfExists<TransformComponent2D>(mRegistry, newEntity, other.mRegistry, eid);
			CopyComponentIfExists<CameraComponent>(mRegistry, newEntity, other.mRegistry, eid);
			CopyComponentIfExists<SpriteRendererComponent>(mRegistry, newEntity, other.mRegistry, eid);
			CopyComponentIfExists<CircleRendererComponent>(mRegistry, newEntity, other.mRegistry, eid);
			CopyComponentIfExists<Rigidbody2DComponent>(mRegistry, newEntity, other.mRegistry, eid);
			CopyComponentIfExists<BoxCollider2DComponent>(mRegistry, newEntity, other.mRegistry, eid);
			CopyComponentIfExists<CircleCollider2DComponent>(mRegistry, newEntity, other.mRegistry, eid);
			CopyComponentIfExists<InputComponent>(mRegistry, newEntity, other.mRegistry, eid);
		}
	}

	Scene& Scene::operator=(const Scene& scene)
	{
		DM_CORE_ASSERT(false, "Not implemented");
		return *this;
	}

	Scene::~Scene()
	{
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		return CreateEntity(UUID(), name);
	}

	Entity Scene::CreateEntity(UUID uuid, const std::string& name)
	{
		Entity entity = { mRegistry.create(), this };
		entity.AddComponent<IDComponent>(uuid);
		entity.AddComponent<TagComponent>(name.empty() ? "Entity" : name);
		entity.AddComponent<TransformComponent2D>();
		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		mRegistry.destroy(entity);
	}

	void Scene::OnRuntimeStart()
	{
		mPhysics2DWorld = new b2World({ 0.0f, -9.8f });

		auto view = mRegistry.view<Rigidbody2DComponent>();
		for (auto eid : view)
		{
			Entity entity = Entity(eid, this);
			auto& transform = entity.GetComponent<TransformComponent2D>();
			auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

			b2BodyDef bodyDef;
			bodyDef.type = DominionRigidbody2DTypeToBox2DBodyType(rb2d.bodyType);
			bodyDef.position.Set(transform.translation.x, transform.translation.y);
			bodyDef.angle = transform.rotation;
			bodyDef.fixedRotation = rb2d.fixedRotation;

			b2Body* physicsBody = mPhysics2DWorld->CreateBody(&bodyDef);
			rb2d.runtimeBody = physicsBody;

			if (entity.HasComponent<BoxCollider2DComponent>())
			{
				auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();

				b2PolygonShape boxShape;
				glm::vec2 size = glm::vec2(bc2d.size.x * transform.scale.x, bc2d.size.y * transform.scale.y);
				glm::vec2 vertices[4];
				vertices[0] = glm::vec2(-size.x + bc2d.offset.x, -size.y + bc2d.offset.y);
				vertices[1] = glm::vec2(size.x + bc2d.offset.x, -size.y + bc2d.offset.y);
				vertices[2] = glm::vec2(size.x + bc2d.offset.x, size.y + bc2d.offset.y);
				vertices[3] = glm::vec2(-size.x + bc2d.offset.x, size.y + bc2d.offset.y);
				boxShape.Set(reinterpret_cast<b2Vec2*>(&vertices), 4);

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &boxShape;
				fixtureDef.density = bc2d.density;
				fixtureDef.friction = bc2d.friction;
				fixtureDef.restitution = bc2d.restitution;
				fixtureDef.restitutionThreshold = bc2d.restitutionThreshold;

				physicsBody->CreateFixture(&fixtureDef);
			}

			if (entity.HasComponent<CircleCollider2DComponent>())
			{
				auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();

				b2CircleShape circleShape;
				circleShape.m_radius = cc2d.radius;
				circleShape.m_p.Set(cc2d.offset.x, cc2d.offset.y);

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &circleShape;
				fixtureDef.density = cc2d.density;
				fixtureDef.friction = cc2d.friction;
				fixtureDef.restitution = cc2d.restitution;
				fixtureDef.restitutionThreshold = cc2d.restitutionThreshold;

				physicsBody->CreateFixture(&fixtureDef);
			}
		}
	}

	void Scene::OnRuntimeStop()
	{
		delete mPhysics2DWorld;
		mPhysics2DWorld = nullptr;
	}

	void Scene::OnUpdateRuntime(Timestep ts)
	{
		// Update scripts
		{
			mRegistry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
			{
				// TODO: Move to Scene::OnScenePlay
				if (!nsc.instance)
				{
					nsc.instance = nsc.InstantiateScript();
					nsc.instance->mEntity = Entity{ entity, this };
					nsc.instance->OnCreate();
				}

				nsc.instance->OnUpdate(ts);
			});
		}

		// Update input component
		{
			mRegistry.view<InputComponent>().each([=](auto entity, InputComponent& ic)
			{
				TagComponent& tc = mRegistry.get<TagComponent>(entity);

				Rigidbody2DComponent* pRbc = mRegistry.has<Rigidbody2DComponent>(entity) ?
					&mRegistry.get<Rigidbody2DComponent>(entity) :
					reinterpret_cast<Rigidbody2DComponent*>(nullptr);

				if (pRbc)
				{
					b2Body* body = reinterpret_cast<b2Body*>(pRbc->runtimeBody);
					if (Input::IsKeyPressed(Key::W))
						body->ApplyForce(b2Vec2(0, ic.verticalSpeed), body->GetWorldCenter(), true);
					else if (Input::IsKeyPressed(Key::S))
						body->ApplyForce(b2Vec2(0, -ic.verticalSpeed), body->GetWorldCenter(), true);

					if (Input::IsKeyPressed(Key::A))
						body->ApplyForce(b2Vec2(-ic.horizontalSpeed, 0), body->GetWorldCenter(), true);
					else if (Input::IsKeyPressed(Key::D))
						body->ApplyForce(b2Vec2(ic.horizontalSpeed, 0), body->GetWorldCenter(), true);
				}
			});
		}

		// Physics
		{
			// TODO: Expose these editor physics settings
			int32_t velocityIterations = 5;
			int32_t positionIterations = 5;

			mPhysics2DWorld->Step(ts, velocityIterations, positionIterations);

			auto view = mRegistry.view<Rigidbody2DComponent>();
			for (auto eid : view)
			{
				Entity entity = Entity(eid, this);
				auto& transform = entity.Transform();
				auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

				b2Body* body = static_cast<b2Body*>(rb2d.runtimeBody);
				const auto& position = body->GetPosition();
				transform.translation.x = position.x;
				transform.translation.y = position.y;
				transform.rotation = body->GetAngle();
			}
		}
	}

	void Scene::OnUpdateEditor(Timestep ts, EditorCamera& camera)
	{
	}

	void Scene::OnUpdateEditor(Timestep ts, const Camera& camera, const glm::mat4& transform)
	{
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		mViewportWidth = width;
		mViewportHeight = height;

		// Resize our non-FixedAspectRatio cameras
		auto view = mRegistry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.fixedAspectRatio)
				cameraComponent.camera.SetViewportSize(width, height);
		}
	}

	Entity Scene::DuplicateEntity(Entity entity)
	{
		Entity newEntity = CreateEntity(entity.Name());

		CopyComponentIfExists<TransformComponent2D>(newEntity, entity);
		CopyComponentIfExists<CameraComponent>(newEntity, entity);
		CopyComponentIfExists<SpriteRendererComponent>(newEntity, entity);
		CopyComponentIfExists<CircleRendererComponent>(newEntity, entity);
		CopyComponentIfExists<Rigidbody2DComponent>(newEntity, entity);
		CopyComponentIfExists<BoxCollider2DComponent>(newEntity, entity);
		CopyComponentIfExists<CircleCollider2DComponent>(newEntity, entity);

		return newEntity;
	}

	Entity Scene::GetPrimaryCameraEntity()
	{
		auto view = mRegistry.view<CameraComponent>();
		for (auto entity : view)
		{
			const auto& camera = view.get<CameraComponent>(entity);
			if (camera.primary)
				return Entity{entity, this};
		}
		return {};
	}

	void Scene::Render(const EditorCamera& editorCamera)
	{
		Renderer2D::BeginScene(editorCamera);
		RenderInternal();
		// mFontRenderer->RenderText("This is sample text", { 25.0f, 25.0f }, 1.0f, glm::vec3(1.0f, 0.3f, 0.2f));
		Renderer2D::EndScene();
	}

	void Scene::Render(const Camera& camera, const glm::mat4& cameraTransform)
	{
		Renderer2D::BeginScene(camera, cameraTransform);
		RenderInternal();
		// mFontRenderer->RenderText("This is sample text", { 25.0f, 25.0f }, 1.0f, glm::vec3(1.0f, 0.3f, 0.2f));
		Renderer2D::EndScene();
	}

	void Scene::RenderInternal()
	{
		// Draw sprites
		{
			auto group = mRegistry.group<TransformComponent2D>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto [transform, sprite] = group.get<TransformComponent2D, SpriteRendererComponent>(entity);
				Renderer2D::DrawSprite(transform.GetTransform(), sprite, static_cast<int>(entity));
			}
		}

		// Draw circles
		{
			auto view = mRegistry.view<TransformComponent2D, CircleRendererComponent>();
			for (auto entity : view)
			{
				auto [transform, circle] = view.get<TransformComponent2D, CircleRendererComponent>(entity);
				Renderer2D::DrawCircle(transform.GetTransform(), circle.color, circle.thickness, circle.fade, (int)entity);
			}
		}
	}

}
