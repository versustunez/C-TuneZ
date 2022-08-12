#include "Application.h"

#include "Input.h"
#include "Renderer/Renderer.h"

namespace TuneZ {
Application::Application() = default;
void Application::Shutdown() {
  m_window.reset();
  glfwTerminate();
}
void Application::Init() {
  m_window = Window::make();
  if (m_window->isValid()) {
    Input::setup(this);
  }
  Renderer::init();
  ImGuiLayer::Init(m_window->native());
}
void Application::Render() {
  glfwPollEvents();
  auto time = (float)glfwGetTime();
  Timestamp timestamp = time - m_lastFrameTime;
  m_lastFrameTime = time;
  m_deltaTime = timestamp;
  m_Scene->onUpdate(timestamp);
  if (m_ImGuiRender) {
    ImGuiLayer::Begin();
    m_Scene->onRenderGUI();
    ImGuiLayer::End();
  }
  glfwSwapBuffers(m_window->native());
  Input::resetMouse();
}
} // namespace TuneZ