#pragma once

#include "Dominion/Core/Base.h"
#include "Dominion/Core/Timestep.h"

#include "Components.h"

#pragma warning (disable: 4267 26439 26495)
#include <entt.hpp>
#pragma warning (default: 4267 26439 26495)

#include <string>

namespace Dominion {

	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity();
		Entity CreateEntity(const std::string& name);

		// TEMP
		entt::registry& Reg() { return m_Registry; }

		void OnUpdate(Timestep timestep);
	private:
		entt::registry m_Registry;

		friend class Entity;
	};

}
