#pragma once
#include "Scene/ImGui.h"
#include "Scene/Scene.h"
#include "Window.h"

#include <TypeDefs.h>

namespace TuneZ {
struct ApplicationData {
  int width{1280};
  int height{720};
};
class Application : public Instance<Application> {
public:
  static ApplicationData &applicationData() {
    static ApplicationData s_applicationData;
    return s_applicationData;
  }
  Application();
  void Shutdown();
  void Init();
  void Render();
  GLFWwindow *NativeWindow() { return m_window->native(); }
  Ref<Scene> &GetScene() { return m_Scene; }
  void SetScene(const Ref<Scene> &scene) { m_Scene = scene; }
  void SetRenderGUI(bool render) { m_ImGuiRender = render; }
  void ToggleRenderGUI() { m_ImGuiRender = !m_ImGuiRender; }
protected:
  friend Window;

  Ref<Scene> m_Scene;
  Ref<Window> m_window;
  float m_lastFrameTime{0.0};
  Timestamp m_deltaTime;
  bool m_ImGuiRender{false};
};

} // namespace TuneZ
