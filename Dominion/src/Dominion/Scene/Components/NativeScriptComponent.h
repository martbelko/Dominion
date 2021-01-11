#pragma once

#include "Dominion/Core/Base.h"

#include "Dominion/Scene/ScriptableEntity.h"

namespace Dominion {

	struct NativeScriptComponent
	{
		ScriptableEntity* instance = nullptr;

		ScriptableEntity* (*InstantiateScript)();

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
		}

		void Destroy()
		{
			instance->OnDestroy();
			delete instance;
		}
	};

}
