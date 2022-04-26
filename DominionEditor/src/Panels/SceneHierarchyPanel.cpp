#include "SceneHierarchyPanel.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <glm/gtc/type_ptr.hpp>

#include "Dominion/Scene/Components.h"

#include "Commands.h"

namespace Dominion {

	extern const std::filesystem::path gAssetPath;

	Ref<Texture2D> g_ResetIcon = nullptr;

	template<typename T>
	static void CombineHash(std::size_t& seed, const T& v)
	{
		std::hash<T> hasher;
		seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}

	SceneHierarchyPanel::SceneHierarchyPanel()
	{
		if (!g_ResetIcon)
			g_ResetIcon = Texture2D::Create("Resources/Icons/Reset.png");
	}

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context, CommandStack& commandStack)
		: SceneHierarchyPanel()
	{
		mCommandStack = &commandStack;
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		mContext = context;
		mSelectionContext = {};
	}

	void SceneHierarchyPanel::SetCommandStack(CommandStack& commandStack)
	{
		mCommandStack = &commandStack;
	}

	void SceneHierarchyPanel::OnImGuiRender(bool allowModify)
	{
		ImGui::Begin("Scene Hierarchy");
		if (mContext)
		{
			mContext->mRegistry.each([&](auto entityID)
				{
					Entity entity{ entityID , mContext.get() };
					DrawEntityNode(entity, allowModify);
				});

			if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
				mSelectionContext = {};

			// Right-click on blank space
			if (allowModify)
			{
				if (ImGui::BeginPopupContextWindow(0, 1, false))
				{
					if (ImGui::MenuItem("Create Empty Entity"))
					{
						Command* command = new AddEntityCommand(mContext, "Empty Entity");
						command->Do();
						mCommandStack->PushCommand(command);
					}

					ImGui::EndPopup();
				}
			}
		}
		ImGui::End();

		ImGui::Begin("Properties");
		if (mSelectionContext)
		{
			DrawComponents(mSelectionContext, allowModify);
		}

		ImGui::End();
	}

	void SceneHierarchyPanel::SetSelectedEntity(Entity entity)
	{
		mSelectionContext = entity;
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity, bool allowModify)
	{
		auto& tag = entity.GetComponent<TagComponent>().tag;

		ImGuiTreeNodeFlags flags = ((mSelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx(reinterpret_cast<void*>(static_cast<uint64_t>(static_cast<uint32_t>(entity))), flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			mSelectionContext = entity;
		}

		bool entityDeleted = false;
		bool entityDuplicated = false;
		if (allowModify && ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
				entityDeleted = true;
			else if (ImGui::MenuItem("Duplicate Entity"))
				entityDuplicated = true;

			ImGui::EndPopup();
		}

		if (opened)
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
			bool opened = ImGui::TreeNodeEx(reinterpret_cast<void*>(9817239), flags, tag.c_str());
			if (opened)
				ImGui::TreePop();
			ImGui::TreePop();
		}

		if (entityDeleted)
		{
			mContext->DestroyEntity(entity);
			if (mSelectionContext == entity)
				mSelectionContext = {};
		}
		else if (entityDuplicated)
		{
			Command* command = new DuplicateEntityCommand(entity);
			command->Do();
			mCommandStack->PushCommand(command);
		}
	}

	template<typename T>
	concept Vectorable = requires(T a)
	{
		std::same_as<T, glm::vec1>;
		std::same_as<T, glm::vec2>;
		std::same_as<T, glm::vec3>;
		std::same_as<T, glm::vec4>;
	};

	template<Vectorable T>
	static void DrawVecControl(const std::string& label, bool allowModify, T& values, const T& resetValue = T(0), const char* format = "%.2f", float columnWidth = 100.0f)
	{
		constexpr uint32_t cols = values.length();

		ImGuiIO& io = ImGui::GetIO();

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		constexpr float spacing = 5.0f;

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		float resetIconSize = lineHeight;

		float maxWidth = std::max(1.0f, ImGui::GetContentRegionAvail().x - spacing * (cols - 1));
		ImGui::PushMultiItemsWidths(cols, maxWidth - resetIconSize - spacing);
		const ImVec4 colors[] = { ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f }, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f }, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f }, ImVec4{ 1.0f, 1.0f, 1.0f, 1.0f } };
		ImVec2 colorSize = { 4.0f, lineHeight };
		for (uint32_t i = 0; i < cols; ++i)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

			const ImVec4& color = colors[i];

			ImGui::PushStyleColor(ImGuiCol_Button, color);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, color);
			ImGui::Button("", colorSize);
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::PopStyleVar();
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ spacing, 0 });

			std::string id = "##" + std::to_string(i);
			if (allowModify)
				ImGui::DragFloat(id.c_str(), &values[i], 0.1f, 0.0f, 0.0f, format);
			else
			{
				float val = values[i];
				ImGui::DragFloat(id.c_str(), &val, 0.1f, 0.0f, 0.0f, format);
			}

			ImGui::PopItemWidth();

			ImGui::SameLine();
			ImGui::PopStyleVar();
		}

		ImGui::SetNextItemWidth(resetIconSize);
		if (ImGui::ImageButton((ImTextureID)g_ResetIcon->GetRendererID(), ImVec2(resetIconSize, resetIconSize), ImVec2(0, 1), ImVec2(1, 0), 0) && allowModify)
		{
			values = resetValue;
		}

		ImGui::Columns(1);
		ImGui::PopID();
	}

	template<typename T>
	static void GetTreeNodeStatus(const std::string& componentName, Entity entity, std::string& description, std::string& error)
	{
		DM_ASSERT(entity.HasComponent<T>(), "Entity does not have this component!");
		if (componentName == "Rigidbody 2D")
		{
			if (!entity.HasComponent<BoxCollider2DComponent>() && !entity.HasComponent<CircleCollider2DComponent>())
			{
				error = "The component also needs Box Collider 2D or Circle Collider 2D component to work correcly";
			}
		}
		else if (componentName == "Input")
		{
			if (!entity.HasComponent<Rigidbody2DComponent>())
			{
				error = "The component also needs Rigid Body 2D component to work correcly";
			}
		}

		if (componentName == "Transform")
		{
			description = "Takes care of position, scale and rotation";
		}
		else if (componentName == "Camera")
		{
			description = "Represents camera";
		}
		else if (componentName == "Sprite Renderer")
		{
			description = "Used for drawing quads with colors and/or textures";
		}
		else if (componentName == "Circle Renderer")
		{
			description = "Used for drawing circles with colors and/or textures";
		}
		else if (componentName == "Rigidbody 2D")
		{
			description = "Adds control of an object's position through physics simulation";
		}
		else if (componentName == "Box Collider 2D")
		{
			description = "Adds rectangular 2D collider";
		}
		else if (componentName == "Circle Collider 2D")
		{
			description = "Adds circle 2D collider";
		}
		else if (componentName == "Input")
		{
			description = "Adds input to entity with RigidBody 2D component";
		}
	}

	template<typename T, std::invocable<T&, bool> UIFunction>
	static void DrawComponent(const std::string& name, Entity entity, bool allowModify, UIFunction uiFunction)
	{
		const ImGuiTreeNodeFlags treeNodeFlags =
			ImGuiTreeNodeFlags_DefaultOpen |
			ImGuiTreeNodeFlags_Framed |
			ImGuiTreeNodeFlags_AllowItemOverlap |
			ImGuiTreeNodeFlags_FramePadding;

		if (entity.HasComponent<T>())
		{
			auto& component = entity.GetComponent<T>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

			ImGui::Separator();

			std::string desc, error;
			GetTreeNodeStatus<T>(name, entity, desc, error);
			const ImVec4 errorColor = ImVec4(1.0f, 0.1f, 0.2f, 1.0f);
			ImVec4 color = error.empty() ? ImVec4(1.0f, 1.0f, 1.0f, 1.0f) : errorColor;
			ImGui::PushStyleColor(ImGuiCol_Text, color);
			bool open = ImGui::TreeNodeEx(reinterpret_cast<void*>(typeid(T).hash_code()), treeNodeFlags, name.c_str());
			ImGui::PopStyleColor();
			ImGui::PopStyleVar();

			if (allowModify)
			{
				ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);

				uint64_t id = 0;
				CombineHash(id, std::hash<std::string>{}(name));
				CombineHash(id, std::hash<uint64_t>{}(entity.GetUUID()));
				ImGui::PushID(id);
				if (ImGui::Button("X", ImVec2{ lineHeight, lineHeight }))
				{
					ImGui::OpenPopup("ComponentSettings");
				}

				if (!desc.empty())
				{
					ImGui::SameLine(contentRegionAvailable.x - lineHeight * 1.5f);
					ImGui::PushStyleColor(ImGuiCol_TextDisabled, ImVec4(0.1f, 0.2f, 1.0f, 1.0f));
					ImGui::TextDisabled("(?)");
					ImGui::PopStyleColor();
					if (ImGui::IsItemHovered())
					{
						ImGui::BeginTooltip();
						ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
						ImGui::TextUnformatted(desc.c_str());
						ImGui::PopTextWrapPos();
						ImGui::EndTooltip();
					}
				}

				if (!error.empty())
				{
					ImGui::SameLine(contentRegionAvailable.x - lineHeight * 2.5f);
					ImGui::PushStyleColor(ImGuiCol_TextDisabled, errorColor);
					ImGui::TextDisabled("(!)");
					ImGui::PopStyleColor();
					if (ImGui::IsItemHovered())
					{
						ImGui::BeginTooltip();
						ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
						ImGui::TextUnformatted(error.c_str());
						ImGui::PopTextWrapPos();
						ImGui::EndTooltip();
					}
				}

				bool removeComponent = false;
				if (ImGui::BeginPopup("ComponentSettings"))
				{
					if (ImGui::MenuItem("Remove component"))
						removeComponent = true;

					ImGui::EndPopup();
				}

				ImGui::PopID();

				if (removeComponent)
					entity.RemoveComponent<T>();
			}

			if (open)
			{
				uiFunction(component, allowModify);
				ImGui::TreePop();
			}
		}
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity, bool allowModify)
	{
		{
			constexpr float rightPadding = 10.0f;
			constexpr const char* addComponentText = "Add Component";
			const float addComponentWidth = ImGui::CalcTextSize(addComponentText).x;

			if (entity.HasComponent<TagComponent>())
			{
				auto& tag = entity.GetComponent<TagComponent>().tag;

				char buffer[256];
				memset(buffer, 0, sizeof(buffer));
				std::strncpy(buffer, tag.c_str(), sizeof(buffer));
				float tagWidth = ImGui::GetContentRegionAvail().x - addComponentWidth - rightPadding;
				ImGui::PushItemWidth(tagWidth);
				if (ImGui::InputText("##Tag", buffer, sizeof(buffer)) && allowModify)
				{
					tag = std::string(buffer);
				}
				ImGui::PopItemWidth();
			}

			ImGui::SameLine();

			ImGui::PushItemWidth(addComponentWidth);
			if (ImGui::Button(addComponentText) && allowModify)
				ImGui::OpenPopup("AddComponent");
			ImGui::PopItemWidth();
		}

		if (allowModify)
		{
			if (ImGui::BeginPopup("AddComponent"))
			{
				if (!mSelectionContext.HasComponent<CameraComponent>())
				{
					if (ImGui::MenuItem("Camera"))
					{
						Command* command = new AddComponentCommand<CameraComponent>(mSelectionContext);
						command->Do();
						mCommandStack->PushCommand(command);
						ImGui::CloseCurrentPopup();
					}
				}

				if (!mSelectionContext.HasComponent<SpriteRendererComponent>())
				{
					if (ImGui::MenuItem("Sprite Renderer"))
					{
						Command* command = new AddComponentCommand<SpriteRendererComponent>(mSelectionContext);
						command->Do();
						mCommandStack->PushCommand(command);
						ImGui::CloseCurrentPopup();
					}
				}

				if (!mSelectionContext.HasComponent<CircleRendererComponent>())
				{
					if (ImGui::MenuItem("Circle Renderer"))
					{
						Command* command = new AddComponentCommand<CircleRendererComponent>(mSelectionContext);
						command->Do();
						mCommandStack->PushCommand(command);
						ImGui::CloseCurrentPopup();
					}
				}

				if (!mSelectionContext.HasComponent<Rigidbody2DComponent>())
				{
					if (ImGui::MenuItem("Rigidbody 2D"))
					{
						Command* command = new AddComponentCommand<Rigidbody2DComponent>(mSelectionContext);
						command->Do();
						mCommandStack->PushCommand(command);
						ImGui::CloseCurrentPopup();
					}
				}

				if (!mSelectionContext.HasComponent<BoxCollider2DComponent>())
				{
					if (ImGui::MenuItem("Box Collider 2D"))
					{
						Command* command = new AddComponentCommand<BoxCollider2DComponent>(mSelectionContext);
						command->Do();
						mCommandStack->PushCommand(command);
						ImGui::CloseCurrentPopup();
					}
				}

				if (!mSelectionContext.HasComponent<CircleCollider2DComponent>())
				{
					if (ImGui::MenuItem("Circle Collider 2D"))
					{
						Command* command = new AddComponentCommand<CircleCollider2DComponent>(mSelectionContext);
						command->Do();
						mCommandStack->PushCommand(command);
						ImGui::CloseCurrentPopup();
					}
				}

				if (!mSelectionContext.HasComponent<InputComponent>())
				{
					if (ImGui::MenuItem("Input"))
					{
						Command* command = new AddComponentCommand<InputComponent>(mSelectionContext);
						command->Do();
						mCommandStack->PushCommand(command);
						ImGui::CloseCurrentPopup();
					}
				}

				ImGui::EndPopup();
			}
		}

		DrawComponent<TransformComponent2D>("Transform", entity, allowModify, [](TransformComponent2D& component, bool allowModify)
		{
			DrawVecControl("Translation", allowModify, component.translation);
			glm::vec1 rotation = glm::vec1(glm::degrees(component.rotation));
			DrawVecControl("Rotation", allowModify, rotation, glm::vec1(0), "%.2f \xc2\xb0");
			component.rotation = glm::radians(rotation.x);
			DrawVecControl("Scale", allowModify, component.scale, glm::vec2(1.0f, 1.0f));
		});

		DrawComponent<CameraComponent>("Camera", entity, allowModify, [](CameraComponent& component, bool allowModify)
		{
			auto& camera = component.camera;

			if (allowModify)
				ImGui::Checkbox("Primary", &component.primary);
			else
			{
				bool val = component.primary;
				ImGui::Checkbox("Primary", &val);
			}

			const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
			const char* currentProjectionTypeString = projectionTypeStrings[static_cast<int>(camera.GetProjectionType())];
			if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
			{
				for (int i = 0; i < 2; ++i)
				{
					bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
					if (ImGui::Selectable(projectionTypeStrings[i], isSelected) && allowModify)
					{
						currentProjectionTypeString = projectionTypeStrings[i];
						camera.SetProjectionType((SceneCamera::ProjectionType)i);
					}

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}

			if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
			{
				float perspectiveVerticalFov = glm::degrees(camera.GetPerspectiveVerticalFOV());
				if (ImGui::DragFloat("Vertical FOV", &perspectiveVerticalFov) && allowModify)
					camera.SetPerspectiveVerticalFOV(glm::radians(perspectiveVerticalFov));

				float perspectiveNear = camera.GetPerspectiveNearClip();
				if (ImGui::DragFloat("Near", &perspectiveNear) && allowModify)
					camera.SetPerspectiveNearClip(perspectiveNear);

				float perspectiveFar = camera.GetPerspectiveFarClip();
				if (ImGui::DragFloat("Far", &perspectiveFar) && allowModify)
					camera.SetPerspectiveFarClip(perspectiveFar);
			}

			if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
			{
				float orthoSize = camera.GetOrthographicSize();
				if (ImGui::DragFloat("Size", &orthoSize) && allowModify)
					camera.SetOrthographicSize(orthoSize);

				float orthoNear = camera.GetOrthographicNearClip();
				if (ImGui::DragFloat("Near", &orthoNear) && allowModify)
					camera.SetOrthographicNearClip(orthoNear);

				float orthoFar = camera.GetOrthographicFarClip();
				if (ImGui::DragFloat("Far", &orthoFar) && allowModify)
					camera.SetOrthographicFarClip(orthoFar);

				if (allowModify)
					ImGui::Checkbox("Fixed Aspect Ratio", &component.fixedAspectRatio);
				else
				{
					bool val = component.fixedAspectRatio;
					ImGui::Checkbox("Fixed Aspect Ratio", &val);
				}
			}
		});

		DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, allowModify, [](SpriteRendererComponent& component, bool allowModify)
		{
			ImGui::ColorEdit4("Color", glm::value_ptr(component.color));

			ImGui::Button("Texture", ImVec2(100.0f, 0.0f));
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
				{
					const wchar_t* path = reinterpret_cast<const wchar_t*>(payload->Data);
					std::filesystem::path texturePath = std::filesystem::path(gAssetPath) / path;
					Ref<Texture2D> texture = Texture2D::Create(texturePath.string());
					if (texture->IsLoaded())
						component.texture = texture;
					else
						DM_WARN("Could not load texture {0}", texturePath.filename().string());
				}
				ImGui::EndDragDropTarget();
			}

			ImGui::DragFloat("Tiling Factor", &component.tilingFactor, 0.1f, 0.0f, 100.0f);
			float val = component.tilingFactor;
			ImGui::DragFloat("Tiling Factor", &val, 0.1f, 0.0f, 100.0f);
		});

		DrawComponent<CircleRendererComponent>("Circle Renderer", entity, allowModify, [](CircleRendererComponent& component, bool allowModify)
		{
			ImGui::ColorEdit4("Color", glm::value_ptr(component.color));
			ImGui::DragFloat("Thickness", &component.thickness, 0.025f, 0.0f, 1.0f);
			ImGui::DragFloat("Fade", &component.fade, 0.00025f, 0.0f, 1.0f);
		});

		DrawComponent<Rigidbody2DComponent>("Rigidbody 2D", entity, allowModify, [](Rigidbody2DComponent& component, bool allowModify)
		{
			const char* bodyTypeStrings[] = { "Static", "Dynamic", "Kinematic" };
			const char* currentbodyTypeString = bodyTypeStrings[static_cast<int>(component.bodyType)];
			if (ImGui::BeginCombo("Body Type", currentbodyTypeString))
			{
				for (int i = 0; i < 2; ++i)
				{
					bool isSelected = currentbodyTypeString == bodyTypeStrings[i];
					if (ImGui::Selectable(bodyTypeStrings[i], isSelected) && allowModify)
					{
						currentbodyTypeString = bodyTypeStrings[i];
						component.bodyType = static_cast<Rigidbody2DComponent::BodyType>(i);
					}

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}

			if (allowModify)
				ImGui::Checkbox("Fixed Rotation", &component.fixedRotation);
			else
			{
				bool val = component.fixedRotation;
				ImGui::Checkbox("Fixed Rotation", &val);
			}
		});

		DrawComponent<BoxCollider2DComponent>("Box Collider 2D", entity, allowModify, [](BoxCollider2DComponent& component, bool allowModify)
		{
			if (allowModify)
			{
				ImGui::DragFloat2("Offset", glm::value_ptr(component.offset));
				ImGui::DragFloat2("Size", glm::value_ptr(component.size));
				ImGui::DragFloat("Density", &component.density, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Friction", &component.friction, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Restitution", &component.restitution, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("RestitutionThreshold", &component.restitutionThreshold, 0.01f, 0.0f);
			}
			else
			{
				BoxCollider2DComponent bc2d = component;

				ImGui::DragFloat2("Offset", glm::value_ptr(bc2d.offset));
				ImGui::DragFloat2("Size", glm::value_ptr(bc2d.size));
				ImGui::DragFloat("Density", &bc2d.density, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Friction", &bc2d.friction, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Restitution", &bc2d.restitution, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("RestitutionThreshold", &bc2d.restitutionThreshold, 0.01f, 0.0f);
			}
		});

		DrawComponent<CircleCollider2DComponent>("Circle Collider 2D", entity, allowModify, [](CircleCollider2DComponent& component, bool allowModify)
		{
			if (allowModify)
			{
				ImGui::DragFloat2("Offset", glm::value_ptr(component.offset));
				ImGui::DragFloat("Radius", &component.radius);
				ImGui::DragFloat("Density", &component.density, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Friction", &component.friction, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Restitution", &component.restitution, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("RestitutionThreshold", &component.restitutionThreshold, 0.01f, 0.0f);
			}
			else
			{
				CircleCollider2DComponent cc2d = component;

				ImGui::DragFloat2("Offset", glm::value_ptr(cc2d.offset));
				ImGui::DragFloat("Radius", &cc2d.radius);
				ImGui::DragFloat("Density", &cc2d.density, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Friction", &cc2d.friction, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Restitution", &cc2d.restitution, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("RestitutionThreshold", &cc2d.restitutionThreshold, 0.01f, 0.0f);
			}
		});

		DrawComponent<InputComponent>("Input", entity, allowModify, [](InputComponent& ic, bool allowModify)
		{
			ImGui::DragFloat("Vertical speed", &ic.verticalSpeed, 0.01f, 0.0f);
			ImGui::DragFloat("Horizontal speed", &ic.horizontalSpeed, 0.01f, 0.0f);
		});
	}

}
