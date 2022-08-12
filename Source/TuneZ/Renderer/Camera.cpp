#include "Camera.h"

#include <Core/Input.h>
#include <GLM.h>
#include <algorithm>
#include <cmath>

namespace TuneZ {
Camera::Camera(double fieldOfView, float zNear, float zFar)
    : m_fov(glm::radians(fieldOfView)),
      m_near{zNear},
      m_far(zFar) {
  m_ConfigItem = Config::Get().GetItemOrDefault("mouse.speed", 0.15);
  updateVectors();
  create();
}

void Camera::update(double dt) {
  handleKeys(dt);
  handleMouse(dt);
  create();
}

void Camera::create() {
  auto &app = Application::applicationData();
  m_aspect = (double)app.width / (double)app.height;
  m_projectionMatrix = glm::perspective(m_fov, m_aspect, m_near, m_far);
  m_viewMatrix = glm::lookAt(m_position, m_position + m_forward, m_up);
  glm::translate(m_viewMatrix, {0,0,-100});
}
void Camera::handleMouse(double dt) {
  if (Input::mouseLocked()) {
    double speed = MOUSE_SPEED * (double)(*m_ConfigItem);
    auto &mouse = Input::mousePos();
    double xOff = speed * -mouse.deltaX;
    double yOff = speed * mouse.deltaY;
    m_mouseRotation.yaw += xOff;
    m_mouseRotation.pitch += yOff;
    m_mouseRotation.pitch = std::clamp(m_mouseRotation.pitch, -89.0, 89.0);
    updateVectors();
  }
}
void Camera::handleKeys(double dt) {
  float speed =
      Input::down(Key::LeftShift) ? CAMERA_SPEED * 4.0 * dt : CAMERA_SPEED * dt;
  if (Input::down(Key::W)) {
    m_position += m_forward * speed;
  } else if (Input::down(Key::S)) {
    m_position -= m_forward * speed;
  }

  if (Input::down(Key::A)) {
    m_position -= m_right * speed;
  } else if (Input::down(Key::D)) {
    m_position += m_right * speed;
  }
  if (Input::down(Key::Q) || Input::down(Key::Space)) {
    m_position += glm::normalize(glm::cross(m_right, m_forward)) * speed;
  } else if (Input::down(Key::E) || Input::down(Key::LeftControl)) {
    m_position -= glm::normalize(glm::cross(m_right, m_forward)) * speed;
  }
}
void Camera::updateVectors() {
  double radiansPitch = glm::radians(m_mouseRotation.pitch);
  double radiansYaw = glm::radians(m_mouseRotation.yaw);
  double cosRPitch = std::cos(radiansPitch);
  glm::vec3 direction;
  direction.x = std::cos(radiansYaw) * cosRPitch;
  direction.y = std::sin(radiansPitch);
  direction.z = std::sin(radiansYaw) * cosRPitch;
  m_forward = glm::normalize(direction);
  m_right = glm::normalize(glm::cross(m_forward, m_up));
}
} // namespace TuneZ