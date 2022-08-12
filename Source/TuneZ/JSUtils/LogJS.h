#pragma once
#include <emscripten/emscripten.h>

extern "C" {
EM_ASYNC_JS(void, pretty_log, (const char *type, const char* module, const char *message), {
  type = UTF8ToString(type);
  message = UTF8ToString(message);
  module = UTF8ToString(module);
  PrettyConsole[type](module, message);
});
}