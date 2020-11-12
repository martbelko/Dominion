#include "SceneHierarchyPanel.h"

namespace Dominion {

	SceneHierarhyPanel::SceneHierarhyPanel(Ref<Scene>& context)
		: m_Context(context)
	{

	}

	void SceneHierarhyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
	}

	void SceneHierarhyPanel::OnImGuiRender()
	{
		if (ImGui::Begin("Scene Hierarchy"))
		{
			m_Context->m_Registry.each([&](auto entityID)
				{
					Entity entity = Entity(entityID, m_Context.get());
					DrawEntityNode(entity);
				});
		}

		ImGui::End();
	}

	void SceneHierarhyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx(reinterpret_cast<void*>(static_cast<uint64_t>(static_cast<uint32_t>(entity))), flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}

		if (opened)
		{
			ImGuiTreeNodeFlags flags =  ImGuiTreeNodeFlags_OpenOnArrow;
			bool opened = ImGui::TreeNodeEx(reinterpret_cast<void*>(9812524151), flags, tag.c_str());
			if (opened)
				ImGui::TreePop();
			ImGui::TreePop();
		}
	}

}
