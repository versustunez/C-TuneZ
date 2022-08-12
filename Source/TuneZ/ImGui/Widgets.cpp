#include "Widgets.h"

#include <ImGui.h>
#include <imgui/imgui_internal.h>

namespace TuneZ {
void Widgets::DrawVec3(const std::string &label, glm::vec3& values,float columnWidth) {

  ImGui::PushID(label.c_str());
  ImGui::Columns(2);
  ImGui::SetColumnWidth(0, columnWidth);
  ImGui::Text("%s", label.c_str());
  ImGui::NextColumn();
  ImGui::DragFloat3("##Drag", glm::value_ptr(values), 0.1f, 0.0f, 0.0f, "%.2f");
  ImGui::Columns(1);
  ImGui::PopID();
}

void Widgets::DrawFloat(const std::string &label, float &value, float columnWidth) {
  ImGui::PushID(label.c_str());
  ImGui::Columns(2);
  ImGui::SetColumnWidth(0, columnWidth);
  ImGui::Text("%s", label.c_str());
  ImGui::NextColumn();
  ImGui::DragFloat("##Drag", &value, 0.1f, 0.0f, 0.0f, "%.2f");
  ImGui::Columns(1);
  ImGui::PopID();
}
} // namespace TuneZ