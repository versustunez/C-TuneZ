#pragma once

#include "Core/Asset.h"
#include "Core/KeyCodes.h"
#include "Core/Timestamp.h"
#include "Renderer/Camera.h"
#include "Renderer/Shader.h"
#include "Renderer/Post-Processing/PostProcessor.h"
#include "Types/UUID.h"

#include <entt/entt.hpp>

namespace TuneZ {
class Entity;
class Scene : public Asset {
public:
  Scene();
  Entity createEntity(const String &name = String());
  Entity createEntityWithUUID(const UUID &uuid, String name = String());
  void destroyEntity(Entity entity);
  void onUpdate(Timestamp ts);
  virtual void onRenderGUI();
  virtual void onCustomUpdate(Timestamp ts);
public:
  virtual void onKeyEvent(KeyCode key);
  virtual void onMouseEvent();
  virtual void onMouseKeyEvent();
protected:
  Ref<Shader> m_baseShader{};
  entt::registry m_registry;
  Map<AssetHandle, Ref<Asset>> m_assets{};
  Ref<Camera> m_camera{};
  Ref<PostProcessor> m_PostProcessing{};

  friend Entity;
  friend class SceneHierarchyPanel;
};
} // namespace TuneZ