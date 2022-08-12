#include "Entity.h"

namespace TuneZ {

Entity::Entity(entt::entity handle, Scene *scene)
    : m_entityHandle(handle),
      m_scene(scene) {}
} // namespace TuneZ