#include "Input.h"

#include "Application.h"
#include "JSUtils/Logger.h"

namespace TuneZ {
bool TuneZ::Input::isLocked = false;

bool Input::down(KeyCode key) {
  auto &keys = KEYS();
  auto state = keys[key];
  return isLocked && (state == KeyState::DOWN || state == KeyState::REPEATED);
}

bool Input::downOnce(KeyCode key) {
  auto &keys = KEYS();
  return isLocked && keys[key] == KeyState::DOWN;
}

MousePos &Input::mousePos() {
  static MousePos s_MousePos;
  return s_MousePos;
}

bool Input::mouseDown(MouseCode key) { return isLocked && MOUSE()[key]; }

static KeyState translateKeyAction(int action) {
  if (action == GLFW_REPEAT)
    return KeyState::REPEATED;
  if (action == GLFW_PRESS)
    return KeyState::DOWN;
  return KeyState::UP;
}

void Input::setup(Application *application) {
  auto *window = application->NativeWindow();
  glfwSetKeyCallback(
      window, [](GLFWwindow *, int key, int scancode, int action, int mods) {
        auto &keys = KEYS();
        keys[key] = translateKeyAction(action);
        triggerKeyEvent(key);
      });
  glfwSetCursorPosCallback(window, [](GLFWwindow *, double x, double y) {
    auto &pos = Input::mousePos();
    if (pos.first) {
      pos.x = x;
      pos.y = y;
      pos.first = false;
    }
    pos.deltaX = pos.x - x;
    pos.deltaY = pos.y - y;
    pos.x = x;
    pos.y = y;
    triggerMouseEvent();
  });
  glfwSetMouseButtonCallback(
      window, [](GLFWwindow *, int button, int action, int mods) {
        auto &mouseData = MOUSE();
        mouseData[button] = action == GLFW_PRESS;
        triggerMouseKeyEvent();
      });
}

Map<MouseCode, int> &Input::MOUSE() {
  static Map<MouseCode, int> s_MouseKeys;
  return s_MouseKeys;
}

Map<KeyCode, KeyState> &Input::KEYS() {
  static Map<KeyCode, KeyState> s_Keys;
  return s_Keys;
}
void Input::resetMouse() {
  auto &mouseData = mousePos();
  mouseData.deltaY = 0;
  mouseData.deltaX = 0;
}

void Input::mouseLock(bool lockState) {
  auto &mouseData = mousePos();
  isLocked = lockState;
  mouseData.isLocked = lockState;
  if (mouseData.isLocked) {
    mouseData.first = true;
  }
}

bool Input::mouseLocked() { return mousePos().isLocked; }

void Input::triggerKeyEvent(KeyCode key) {
  Application::Get().GetScene()->onKeyEvent(key);
}

void Input::triggerMouseEvent() { Application::Get().GetScene()->onMouseEvent(); }

void Input::triggerMouseKeyEvent() {
  Application::Get().GetScene()->onMouseKeyEvent();
}
} // namespace TuneZ