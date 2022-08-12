#pragma once

#include "Core/Asset.h"
#include "Core/Timestamp.h"
#include "Types/UUID.h"

#include <GLM.h>
#include <utility>

namespace TuneZ {
class Entity;
struct IDComponent {
  UUID ID;

  IDComponent() = default;
  IDComponent(const IDComponent &) = default;
};

struct TagComponent {
  String tag;

  TagComponent() = default;
  TagComponent(const TagComponent &) = default;
  explicit TagComponent(String _tag) : tag(std::move(_tag)) {}
};

struct TransformComponent {
  glm::vec3 translation = {0.0f, 0.0f, 0.0f};
  glm::vec3 rotation = {0.0f, 0.0f, 0.0f};
  glm::vec3 scale = {1.0f, 1.0f, 1.0f};

  TransformComponent() = default;
  TransformComponent(const TransformComponent &) = default;
  explicit TransformComponent(const glm::vec3 &_translation)
      : translation(_translation) {}

  glm::mat4 transform() const {
    glm::mat4 _rotation = glm::toMat4(glm::quat(rotation));
    return glm::translate(glm::mat4(1.0f), translation) * _rotation *
           glm::scale(glm::mat4(1.0f), scale);
  }
};

struct InstancedMeshComponent {
  AssetHandle asset;
};

struct ModelComponent {
  AssetHandle asset{};
};

struct LightComponent {
  glm::vec3 Position{0.0};
  glm::vec3 Color{1.0};
  // Holds Radius, Linear, Quadratic
  glm::vec4 MetaData{1.0, 0.7, 1.8, 1.0};
};

struct UpdateComponent {
  explicit UpdateComponent(
      std::function<void(Entity entity, Timestamp ts)> updateFnc)
      : update(std::move(updateFnc)) {}
  std::function<void(Entity entity, Timestamp ts)> update;
};

struct KeyEventListenerComponent {
  explicit KeyEventListenerComponent(std::function<void()> updateFnc)
      : update(std::move(updateFnc)) {}
  std::function<void()> update;
};

struct MouseEventListenerComponent {
  explicit MouseEventListenerComponent(std::function<void()> updateFnc)
      : update(std::move(updateFnc)) {}
  std::function<void()> update;
};
} // namespace TuneZ