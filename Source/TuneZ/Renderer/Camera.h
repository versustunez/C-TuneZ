#pragma once
#include "Core/Config.h"

#include <GLM.h>
#define CAMERA_SPEED 1.0
#define MOUSE_SPEED 5.0

namespace TuneZ {
struct MouseRotation {
  double yaw{-90.0};
  double pitch{0.0};
};
class Camera {
public:
  explicit Camera(double fieldOfView = 60, float zNear = 0.01,
                  float zFar = 4000);
  void update(double dt);

  glm::mat4 &view() { return m_viewMatrix; };
  glm::mat4 &projection() { return m_projectionMatrix; };
  glm::vec3 position() { return m_position; }

  void create();
  void handleKeys(double dt);
  void handleMouse(double dt);

protected:
  glm::vec3 m_position{0.0, 0.0, 10.0};
  glm::vec3 m_forward{0.0, 0.0, -1.0};
  glm::vec3 m_up{0.0, 1.0, 0.0};
  glm::vec3 m_right{0.0, 1.0, 0.0};
  double m_fov{};
  double m_near{};
  double m_far{};
  double m_aspect;
  glm::mat4 m_projectionMatrix;
  glm::mat4 m_viewMatrix;
  MouseRotation m_mouseRotation{};
  ConfigItem* m_ConfigItem;
  void updateVectors();
};
} // namespace TuneZ