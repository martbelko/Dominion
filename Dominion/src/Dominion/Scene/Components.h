#pragma once

#include "Dominion/Core/Base.h"
#include "Dominion/Scene/SceneCamera.h"

#include <glm/glm.hpp>

#include <string>

namespace Dominion {

	class ScriptableEntity;

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}
	};

	struct TransformComponent
	{
		glm::mat4 Transform{ 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& transform)
			: Transform(transform) {}

		operator glm::mat4&() { return Transform; }
		operator const glm::mat4&() const { return Transform; }
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) {}
	};

	struct CameraComponent
	{
		SceneCamera Cam;
		bool Primary = false;
		bool FixedAspectRation = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		std::function<void()> InstantiateFunction;
		std::function<void()> DestroyInstanceFunction;

		std::function<void()> OnCreateFunction;
		std::function<void()> OnDestroyFunction;
		std::function<void(Timestep)> OnUpdateFunction;

		template<typename T>
		void Bind()
		{
			InstantiateFunction = [&]() { Instance = new T(); };
			DestroyInstanceFunction = [&]() { delete static_cast<T*>(Instance); };
			OnCreateFunction = [&]() { static_cast<T*>(Instance)->OnCreate(); };
			OnDestroyFunction = [&]() { static_cast<T*>(Instance)->OnDestroy(); };
			OnUpdateFunction = [&](Timestep ts) { static_cast<T*>(Instance)->OnUpdate(ts); };
		}
	};

}
