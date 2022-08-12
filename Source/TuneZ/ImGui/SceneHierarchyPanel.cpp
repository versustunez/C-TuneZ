#include "Widgets.h"

#include <GLM.h>
#include <ImGui.h>
#include <imgui/imgui_internal.h>

namespace TuneZ {

void SceneHierarchyPanel::OnRender() {
  ImGui::Begin("Scene Hierarchy");
  if (m_Scene) {
    m_Scene->m_registry.each([&](auto entityID) {
      Entity entity{entityID, m_Scene};
      DrawEntityNode(entity);
    });
    if (ImGui::BeginPopupContextItem()) {
      if (ImGui::MenuItem("Add Entity"))
        m_Scene->createEntity();
    }
  }
  ImGui::End();

  if (m_SelectionContext) {
    ImGui::Begin("Properties");
    DrawComponents(m_SelectionContext);
    ImGui::End();
  }
}
void SceneHierarchyPanel::DrawEntityNode(Entity entity) {
  auto &tag = entity.getComponent<TagComponent>().tag;

  ImGuiTreeNodeFlags flags =
      ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) |
      ImGuiTreeNodeFlags_OpenOnArrow;
  flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
  bool opened = ImGui::TreeNodeEx((void *)(uint64_t)(uint32_t)entity, flags,
                                  "%s", tag.c_str());
  if (ImGui::IsItemClicked()) {
    m_SelectionContext = entity;
  }

  if (opened) {
    ImGui::TreePop();
  }
  bool entityDeleted = false;
  if (ImGui::BeginPopupContextItem()) {
    if (ImGui::MenuItem("Delete Entity"))
      entityDeleted = true;

    ImGui::EndPopup();
  }

  if (entityDeleted) {
    m_Scene->destroyEntity(entity);
    if (m_SelectionContext == entity)
      m_SelectionContext = {};
  }
}

template <typename T>
void SceneHierarchyPanel::DisplayAddComponentEntry(
    const std::string &entryName) {
  if (!m_SelectionContext.hasComponent<T>()) {
    if (ImGui::MenuItem(entryName.c_str())) {
      m_SelectionContext.addComponent<T>();
      ImGui::CloseCurrentPopup();
    }
  }
}

template <typename T, typename UIFunction>
static void DrawComponent(const std::string &name, Entity entity,
                          UIFunction uiFunction) {
  const ImGuiTreeNodeFlags treeNodeFlags =
      ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed |
      ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap |
      ImGuiTreeNodeFlags_FramePadding;
  if (entity.hasComponent<T>()) {
    auto &component = entity.getComponent<T>();
    ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{4, 4});
    float lineHeight =
        GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
    ImGui::Separator();
    bool open = ImGui::TreeNodeEx((void *)typeid(T).hash_code(), treeNodeFlags,
                                  "%s", name.c_str());
    ImGui::PopStyleVar();
    ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
    if (ImGui::Button("+", ImVec2{lineHeight, lineHeight})) {
      ImGui::OpenPopup("ComponentSettings");
    }

    bool removeComponent = false;
    if (ImGui::BeginPopup("ComponentSettings")) {
      if (ImGui::MenuItem("Remove component"))
        removeComponent = true;

      ImGui::EndPopup();
    }

    if (open) {
      uiFunction(component);
      ImGui::TreePop();
    }

    if (removeComponent)
      entity.removeComponent<T>();
  }
}

void SceneHierarchyPanel::DrawComponents(Entity entity) {
  if (entity.hasComponent<TagComponent>()) {
    auto &tag = entity.getComponent<TagComponent>().tag;

    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    std::strncpy(buffer, tag.c_str(), sizeof(buffer));
    if (ImGui::InputText("##Tag", buffer, sizeof(buffer))) {
      tag = std::string(buffer);
    }
  }

  ImGui::SameLine();
  ImGui::PushItemWidth(-1);

  if (ImGui::Button("Add Component"))
    ImGui::OpenPopup("AddComponent");

  if (ImGui::BeginPopup("AddComponent")) {
    DisplayAddComponentEntry<LightComponent>("Light");
    DisplayAddComponentEntry<InstancedMeshComponent>("Mesh");
    ImGui::EndPopup();
  }
  DrawComponent<TransformComponent>(
      "Transform", entity, [](TransformComponent &component) {
        Widgets::DrawVec3("Translation", component.translation);
        glm::vec3 rotation = glm::degrees(component.rotation);
        Widgets::DrawVec3("Rotation", rotation);
        component.rotation = glm::radians(rotation);
        Widgets::DrawVec3("Scale", component.scale);
      });
  DrawComponent<LightComponent>("Light", entity, [](LightComponent &component) {
    Widgets::DrawVec3("Positon", component.Position);
    ImGui::ColorEdit3("Color", glm::value_ptr(component.Color), 0);
    Widgets::DrawFloat("Intensity", component.MetaData.w);
    Widgets::DrawFloat("Linear", component.MetaData.y);
    Widgets::DrawFloat("Quadratic", component.MetaData.z);
    component.MetaData = glm::max(glm::vec4(0), component.MetaData);
  });
  DrawComponent<InstancedMeshComponent>(
      "Mesh", entity, [](InstancedMeshComponent &component) {
        auto &manager = MeshManager::Get();
        AssetHandle current = component.asset;
        if (ImGui::BeginCombo("Projection",manager.AssetToName[current].c_str())) {
          for (auto &[asset, name] : manager.AssetToName) {
            bool isSelected = current == asset;
            if (ImGui::Selectable(name.c_str(), isSelected)) {
              component.asset = asset;
            }
            if (isSelected)
              ImGui::SetItemDefaultFocus();
          }
          ImGui::EndCombo();
        }
      });
  ImGui::PopItemWidth();
}
} // namespace TuneZ
