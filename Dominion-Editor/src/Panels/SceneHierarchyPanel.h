#pragma once

#include <Dominion.h>
#include <DominionImGui.h>

namespace Dominion {

	class Scene;

	class SceneHierarhyPanel
	{
	public:
		SceneHierarhyPanel(const Ref<Scene>& context = nullptr);

		void SetContext(const Ref<Scene>& context);
		void SetSelectedEntity(Entity entity) { m_SelectionContext = entity; }

		void OnImGuiRender();
	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);
	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	private:
		static Ref<Texture2D> s_DefaultTexture;
	};

}
