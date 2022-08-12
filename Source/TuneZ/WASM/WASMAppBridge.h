#pragma once
#include "Core/Application.h"
#include "Core/Input.h"

#include <emscripten/emscripten.h>

using namespace TuneZ;

extern "C" {

EMSCRIPTEN_KEEPALIVE
void app_set_dimensions(int width, int height) {
  auto& data = Application::applicationData();
  data.width = width;
  data.height = height;
}

EMSCRIPTEN_KEEPALIVE
void app_set_lock(bool isLocked) {
  Input::mouseLock(isLocked);
}

EMSCRIPTEN_KEEPALIVE
const char* app_save_config() {
  // This is emscripten callback :D
  return Config::Get().Dump().data();
}

EMSCRIPTEN_KEEPALIVE
void app_restore_config(const String& value) {
  Config::Get().Restore(value);
}

EMSCRIPTEN_KEEPALIVE
void app_toggle_render_gui() {
  Application::Get().ToggleRenderGUI();
}
}