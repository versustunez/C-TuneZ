#pragma once

#include "Timestamp.h"

#include <GL.h>
#include <TypeDefs.h>

namespace TuneZ {
class Window {
public:
  static Ref<Window> make();
  GLFWwindow* native() { return m_window; }
  ~Window();
  bool isOpen() { return !glfwWindowShouldClose(m_window); }
  bool isValid() { return m_window != nullptr; }
  Window() = default;
protected:
  void create();
  GLFWwindow *m_window{nullptr};
  static void printGLInfo();
};
} // namespace TuneZ
