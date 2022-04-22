#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "SceneCamera.h"
#include "Dominion/Renderer/Texture.h"
#include "Dominion/Core/UUID.h"

namespace Dominion {

	// Forward declarations
	class ScriptableEntity;

	// TODO: Rule of 5 for ALL components

	struct IDComponent
	{
		UUID uuid;
	};

	struct TagComponent
	{
		std::string tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: tag(tag) {}
	};

	struct TransformComponent2D
	{
		glm::vec2 translation = { 0.0f, 0.0f };
		glm::vec2 scale = { 1.0f, 1.0f };
		float rotation = 0.0f;

		TransformComponent2D() = default;
		TransformComponent2D(const TransformComponent2D&) = default;
		TransformComponent2D(const glm::vec2& translation)
			: translation(translation) {}

		glm::mat4 GetTransform() const
		{
			glm::mat4 rotationMatrix = glm::toMat4(glm::quat(glm::vec3(0.0f, 0.0f, rotation)));

			return glm::translate(glm::mat4(1.0f), glm::vec3(translation, 0.0f))
				* rotationMatrix
				* glm::scale(glm::mat4(1.0f), glm::vec3(scale, 1.0f));
		}
	};

	struct SpriteRendererComponent
	{
		glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };
		Ref<Texture2D> texture;
		float tilingFactor = 1.0f;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: color(color) {}
	};

	struct CircleRendererComponent
	{
		glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };
		float thickness = 1.0f;
		float fade = 0.005f;

		CircleRendererComponent() = default;
		CircleRendererComponent(const CircleRendererComponent&) = default;
		CircleRendererComponent(const glm::vec4& color)
			: color(color) {}
	};

	struct CameraComponent
	{
		SceneCamera camera;
		bool primary = true; // TODO: think about moving to Scene
		bool fixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	struct NativeScriptComponent
	{
		ScriptableEntity* instance = nullptr;

		ScriptableEntity*(*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc)
			{
				delete nsc->instance;
				nsc->instance = nullptr;
			};
		}
	};

	// Physics

	struct Rigidbody2DComponent
	{
		enum class BodyType
		{
			Static = 0,
			Dynamic,
			Kinematic
		};

		BodyType bodyType = BodyType::Static;
		bool fixedRotation = false;

		// Storage for runtime
		void* runtimeBody = nullptr;
	};

	struct BoxCollider2DComponent
	{
		glm::vec2 offset = glm::vec2(0.0f, 0.0f);
		glm::vec2 size = glm::vec2(0.5f, 0.5f);

		// TODO: move into physics material
		float density = 1.0f;
		float friction = 0.5f;
		float restitution = 0.0f;
		float restitutionThreshold = 0.5f;

		// Storage for runtime
		void* runtimeFixture = nullptr;
	};

	struct CircleCollider2DComponent
	{
		glm::vec2 offset = glm::vec2(0.0f, 0.0f);
		float radius = 0.5f;

		// TODO: move into physics material
		float density = 1.0f;
		float friction = 0.5f;
		float restitution = 0.0f;
		float restitutionThreshold = 0.5f;

		// Storage for runtime
		void* runtimeFixture = nullptr;
	};

	struct InputComponent
	{
		float verticalSpeed = 20.0f;
		float horizontalSpeed = 20.0f;
	};

}
