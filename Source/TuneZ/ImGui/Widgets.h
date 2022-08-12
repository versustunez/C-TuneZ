#pragma once

#include "Scene/Entity.h"
#include "Scene/Scene.h"

namespace TuneZ {
class Widgets {
public:
  static void DrawVec3(const std::string& label, glm::vec3& values, float columnWidth = 100.0f);
  static void DrawFloat(const std::string& label, float& value, float columnWidth = 100.0f);
};
class SceneHierarchyPanel {
public:
  void SetScene(Scene *scene) {
    m_Scene = scene;
    m_SelectionContext = {};
  }
  void OnRender();
protected:
  template<typename T>
  void DisplayAddComponentEntry(const std::string& entryName);
  void DrawEntityNode(Entity entity);
  void DrawComponents(Entity entity);
protected:
  Scene *m_Scene{nullptr};
  Entity m_SelectionContext;
};
} // namespace TuneZ