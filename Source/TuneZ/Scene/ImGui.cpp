#include "ImGui.h"

namespace TuneZ {

void ImGuiLayer::Init(GLFWwindow* window) {
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  ImGui::StyleColorsDark();
  SetColorTheme();
  SetStyle();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  const char* glsl_version = "#version 300 es";
  ImGui_ImplOpenGL3_Init(glsl_version);
}
void ImGuiLayer::Begin() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}
void ImGuiLayer::End() {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
void ImGuiLayer::SetColorTheme() {
  auto& colors = ImGui::GetStyle().Colors;
  colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

  // Headers
  colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
  colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
  colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

  // Buttons
  colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
  colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
  colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

  // Frame BG
  colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
  colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
  colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

  // Tabs
  colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
  colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
  colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
  colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
  colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

  // Title
  colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
  colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
  colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
}
void ImGuiLayer::SetStyle() {
  auto& style = ImGui::GetStyle();
  style.FrameRounding = 2;
  style.WindowPadding = ImVec2(5,5);
  style.FramePadding = ImVec2(5,5);
  style.CellPadding = ImVec2(5,5);
  style.ItemSpacing = ImVec2(5,5);
  style.ItemInnerSpacing = ImVec2(5,5);
  style.IndentSpacing = 5;
  style.ScrollbarSize = 10;
  style.GrabMinSize = 10;
  style.WindowBorderSize = 0;

  style.WindowMinSize = ImVec2(500, 300);
}
void ImGuiLayer::restoreIni() {
  // @TODO: Load File from SessionStorage ;)
/*  FS::writeInternal("/tmp/imgui.ini", FS::readFile("./out/ws/imgui.ini"));
  ImGui::LoadIniSettingsFromDisk("/tmp/imgui.ini");*/
}
void ImGuiLayer::saveIni() {
  // @TODO: Save File to SessionStorage ;)
/*  ImGui::SaveIniSettingsToDisk("/tmp/imgui.ini");
  auto content = FS::readInternal("/tmp/imgui.ini");
  FS::writeFile("./out/ws/imgui.ini", content);*/
}
}