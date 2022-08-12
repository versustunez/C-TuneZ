#pragma once

#include "Application.h"
#include "KeyCodes.h"

#include <TypeDefs.h>

namespace TuneZ {
enum class KeyState { UP = 0, DOWN, REPEATED };
struct MousePos {
  double x{0};
  double y{0};
  double deltaX{0};
  double deltaY{0};
  bool first{true};
  bool isLocked{false};
};
class Input {
public:
  static bool down(KeyCode key);
  static bool downOnce(KeyCode key);
  static MousePos &mousePos();
  static bool mouseDown(MouseCode key);
  static bool mouseLocked();
  static void setup(Application* application);
  static void resetMouse();
  static void mouseLock(bool lock);
  static bool isLocked;
protected:
  static Map<MouseCode, int>& MOUSE();
  static Map<KeyCode, KeyState>& KEYS();
  static void triggerKeyEvent(KeyCode key);
  static void triggerMouseEvent();
  static void triggerMouseKeyEvent();
};
} // namespace TuneZ