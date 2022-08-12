#pragma once

#include "Components.h"
#include "Scene.h"

#include <entt/entt.hpp>

namespace TuneZ {

class Entity {
public:
  Entity() = default;
  Entity(entt::entity handle, Scene *scene);
  Entity(const Entity &other) = default;

  template <typename T, typename... Args> T &addComponent(Args &&...args) {
    T &component = m_scene->m_registry.emplace<T>(m_entityHandle,
                                                  std::forward<Args>(args)...);
    return component;
  }

  template <typename T, typename... Args>
  T &addOrReplaceComponent(Args &&...args) {
    T &component = m_scene->m_registry.emplace_or_replace<T>(
        m_entityHandle, std::forward<Args>(args)...);
    return component;
  }

  template <typename T> T &getComponent() {
    return m_scene->m_registry.get<T>(m_entityHandle);
  }

  template <typename T> bool hasComponent() {
    return m_scene->m_registry.all_of<T>(m_entityHandle);
  }

  template <typename T> void removeComponent() {
    m_scene->m_registry.remove<T>(m_entityHandle);
  }

  operator bool() const { return m_entityHandle != entt::null; }
  operator entt::entity() const { return m_entityHandle; }
  operator uint32_t() const { return (uint32_t)m_entityHandle; }

  UUID getUUID() { return getComponent<IDComponent>().ID; }
  const String &getName() { return getComponent<TagComponent>().tag; }

  bool operator==(const Entity &other) const {
    return m_entityHandle == other.m_entityHandle && m_scene == other.m_scene;
  }

  bool operator!=(const Entity &other) const { return !(*this == other); }

private:
  entt::entity m_entityHandle{entt::null};
  Scene *m_scene = nullptr;
};

} // namespace TuneZ