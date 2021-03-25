#pragma once

#include "Dominion/Core/Base.h"

#include <entt.hpp>

namespace Dominion {

	// Forward declarations
	class Scene;

	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity entityHandle, Scene* scene);
		Entity(U32 entityHandle, Scene* scene) : Entity(static_cast<entt::entity>(entityHandle), scene) {}
		Entity(const Entity&) = default;
		virtual ~Entity() = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			DM_CORE_ASSERT(!HasComponent<T>(), "Entity already has this component!");
			T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
			m_Scene->OnComponentAdded<T>(*this, component);
			return component;
		}

		template<typename T, typename... Args>
		T& GetOrAddComponent(Args&&... args)
		{
			T& component = m_Scene->m_Registry.get_or_emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
			m_Scene->OnComponentAdded<T>(*this, component);
			return component;
		}

		template<typename T>
		void RemoveComponent()
		{
			DM_CORE_ASSERT(HasComponent<T>(), "Entity does not have this component!");
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		template<typename... T>
		decltype(auto) GetComponent()
		{
			DM_CORE_ASSERT(HasComponent<T...>(), "Entity does not have these components!");
			return m_Scene->m_Registry.get<T...>(m_EntityHandle);
		}

		template<typename... T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.has<T...>(m_EntityHandle);
		}

		template<typename... T>
		bool HasAnyComponent()
		{
			return m_Scene->m_Registry.any<T...>(m_EntityHandle);
		}

		operator bool() const { return m_EntityHandle != entt::null; }
		operator U32() const { return static_cast<uint32_t>(m_EntityHandle); }

		U32 GetID() const { return static_cast<uint32_t>(m_EntityHandle); }

		bool operator==(const Entity& other) const;
		bool operator!=(const Entity& other) const;
	private:
		void Destroy();
		void RemoveAllComponents();
	private:
		entt::entity m_EntityHandle{ entt::null };
		Scene* m_Scene = nullptr;

		friend class Scene;
	};

}
