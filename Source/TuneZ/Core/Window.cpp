#include "Window.h"

#include "Application.h"
#include "JSUtils/Logger.h"

namespace TuneZ {
void glfw_error_callback(int error, const char *description) {
  ERROR("Error {} >> {}", error, description);
}

Ref<Window> Window::make() {
  auto window = CreateRef<Window>();
  window->create();
  return window;
}

void Window::create() {
  auto &appData = Application::applicationData();
  if (!glfwInit())
    return;
  glfwSetErrorCallback(glfw_error_callback);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, 0);
  m_window = glfwCreateWindow(appData.width, appData.height, "TuneZ", nullptr, nullptr);
  if (!m_window)
    return;
  glfwMakeContextCurrent(m_window);
  printGLInfo();
}
Window::~Window() {
  if (m_window)
    glfwDestroyWindow(m_window);
}
void Window::printGLInfo() {
  LOG("OpenGL Info:");
  LOG("  Vendor: {}", glGetString(GL_VENDOR));
  LOG("  Renderer: {}", glGetString(GL_RENDERER));
  LOG("  Version: {}", glGetString(GL_VERSION));
}
} // namespace TuneZ