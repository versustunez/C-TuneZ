#pragma once

#include "Audio/FIFOBuffer.h"
#include "Scene/Components.h"
#include "Scene/Scene.h"
#include "ImGui/Widgets.h"

namespace TuneZ {
struct AudioPoint {
  float goalY{0.0};
  TransformComponent* transform{};
};
class MainScene : public Scene {
public:
  MainScene();
  void onCustomUpdate(Timestamp ts) override;
  void onRenderGUI() override;
  FIFOBuffer* m_Buffer = &FIFOBuffer::Get();
protected:
  Vector<AudioPoint> m_Points;
  SceneHierarchyPanel m_EntityPanel;
};
} // namespace TuneZ