#pragma once

#include "Mesh.h"
#include "Shader.h"

#include <TypeDefs.h>
#include <GLM.h>

namespace TuneZ {
struct RenderingItem {
  Mesh* mesh{ nullptr};
  int count{0};
  Vector<glm::mat4> transforms{};
};
class InstanceRenderer : public Instance<InstanceRenderer> {
public:
  void begin(const Ref<Shader>& shader);
  void end();
  void submit(Mesh* mesh, glm::mat4 transform);
protected:
  void flush();
  void flushAsset(AssetHandle mesh);
  void init();
  size_t m_index{0};
  Map<AssetHandle, RenderingItem> m_items;
  bool m_isInited{false};
};
}