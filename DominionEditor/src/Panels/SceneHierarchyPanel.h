#pragma once

#include "Dominion/Core/Base.h"
#include "Dominion/Scene/Scene.h"
#include "Dominion/Scene/Entity.h"

#include "CommandStack.h"

namespace Dominion {

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel();
		SceneHierarchyPanel(const Ref<Scene>& scene, CommandStack& commandStack);

		void SetContext(const Ref<Scene>& scene);
		void SetCommandStack(CommandStack& commandStack);

		void OnImGuiRender(bool allowModify = true);

		Entity GetSelectedEntity() const { return mSelectionContext; }
		void SetSelectedEntity(Entity entity);
	private:
		void DrawEntityNode(Entity entity, bool allowModify);
		void DrawComponents(Entity entity, bool allowModify);
	private:
		Ref<Scene> mContext;
		Entity mSelectionContext;
		CommandStack* mCommandStack = nullptr;
	};

}
