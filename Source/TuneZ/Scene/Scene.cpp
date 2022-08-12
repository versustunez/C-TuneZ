#include "Scene.h"

#include "Entity.h"
#include "Renderer/InstanceRenderer.h"
#include "Renderer/Mesh.h"
#include "Renderer/Renderer.h"

namespace TuneZ {

Scene::Scene() {
  m_camera = CreateRef<Camera>();
  m_PostProcessing = CreateRef<PostProcessor>();
  m_PostProcessing->InitPipeline();
}

Entity Scene::createEntity(const String &name) {
  return createEntityWithUUID(UUID(), name);
}
Entity Scene::createEntityWithUUID(const UUID &uuid, String name) {
  Entity entity = {m_registry.create(), this};
  entity.addComponent<IDComponent>(uuid);
  entity.addComponent<TransformComponent>();
  auto &tag = entity.addComponent<TagComponent>();
  tag.tag = name.empty() ? "Entity" : name;
  return entity;
}
void Scene::destroyEntity(Entity entity) { m_registry.destroy(entity); }
void Scene::onUpdate(Timestamp ts) {
  if (!m_baseShader)
    return;
  m_camera->update(ts);
  auto &assetManager = AssetManager::Get();
  auto updateEntities = m_registry.view<UpdateComponent>();
  onCustomUpdate(ts);
  for (auto entity : updateEntities) {
    auto updateComponent = updateEntities.get<UpdateComponent>(entity);
    Entity internalEntity{entity, this};
    updateComponent.update(internalEntity, ts);
  }
  Renderer::clearColor({0, 0, 0, 1});
  Renderer::clear();
  m_PostProcessing->BeginPipeline();
  Renderer::clearColor({0, 0, 0, 1});
  Renderer::clear();
  Renderer::beginScene(m_camera);
  {
    auto renderer = InstanceRenderer::Get();
    renderer.begin(m_baseShader);
    auto view = m_registry.view<TransformComponent, InstancedMeshComponent>();
    for (auto entity : view) {
      auto [transform, mesh] =
          view.get<TransformComponent, InstancedMeshComponent>(entity);
      auto *toRenderMesh = assetManager.asset(mesh.asset)->as<Mesh>();
      if (mesh.asset) {
        renderer.submit(toRenderMesh, transform.transform());
      }

    }
    renderer.end();
  }
  {
    auto view = m_registry.view<LightComponent>();
    Vector<LightComponent> Lights;
    for (auto entity : view) {
      Lights.push_back(view.get<LightComponent>(entity));
    }
    m_PostProcessing->GetLightPass()->SetLights(Lights);
  }
  m_PostProcessing->ApplyProcessing();
}

void Scene::onRenderGUI() {

}

void Scene::onCustomUpdate(Timestamp ts) {}

void Scene::onKeyEvent(KeyCode) {
  // fetch all Entities with KeyEventComponents
  auto view = m_registry.view<KeyEventListenerComponent>();
  for (auto entity : view) {
    auto &eventListenerComponent = view.get<KeyEventListenerComponent>(entity);
    eventListenerComponent.update();
  }
}
void Scene::onMouseEvent() {
  auto view = m_registry.view<MouseEventListenerComponent>();
  for (auto entity : view) {
    auto &eventListenerComponent =
        view.get<MouseEventListenerComponent>(entity);
    eventListenerComponent.update();
  }
}
void Scene::onMouseKeyEvent() {
  auto view = m_registry.view<MouseEventListenerComponent>();
  for (auto entity : view) {
    auto &eventListenerComponent =
        view.get<MouseEventListenerComponent>(entity);
    eventListenerComponent.update();
  }
}

} // namespace TuneZ
