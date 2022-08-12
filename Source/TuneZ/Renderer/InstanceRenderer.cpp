#include "InstanceRenderer.h"

#include "Mesh.h"
#include "Renderer.h"

#define MAX_INSTANCE_ITEMS 500

namespace TuneZ {

void InstanceRenderer::begin(const Ref<Shader> &shader) {
  if (!m_isInited)
    init();
  flush();
  shader->use();
  shader->setMat4("u_ViewProjection", Renderer::s_SceneData->Matrix);
}

void InstanceRenderer::end() { flush(); }

void InstanceRenderer::submit(Mesh *mesh, glm::mat4 transform) {
  //
  auto &item = m_items[mesh->handle()];
  if (item.mesh == nullptr) {
    item.mesh = mesh;
    m_index++;
  }
  // FLUSH Item and move on
  if (item.count == MAX_INSTANCE_ITEMS) {
    flushAsset(mesh->handle());
    item.count = 0;
    item.transforms.clear();
  }
  item.count++;
  item.transforms.push_back(transform);
}

void InstanceRenderer::flush() {
  // submit to GPU and reset state
  if (m_index > 0) {
    for (auto &[key, item] : m_items)
      flushAsset(key);
  }
  m_items.clear();
  m_index = 0;
}
void InstanceRenderer::flushAsset(AssetHandle handle) {
  auto &item = m_items[handle];
  auto *mesh = item.mesh;
  mesh->bind();
  mesh->setData(item.transforms);
  auto size = (GLint)mesh->size();
  glDrawElementsInstanced(GL_TRIANGLES, size, GL_UNSIGNED_INT,
                          nullptr, item.count);
}
void InstanceRenderer::init() {
  if (m_isInited)
    return;
  m_isInited = true;
}

} // namespace TuneZ