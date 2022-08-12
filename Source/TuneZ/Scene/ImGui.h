#pragma once
#include <ImGui.h>
namespace TuneZ {
class ImGuiLayer {
public:
  static void Init(GLFWwindow* window);
  static void Begin();
  static void End();
  static void restoreIni();
  static void saveIni();
protected:
  static void SetColorTheme();
  static void SetStyle();
};
}
