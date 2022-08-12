#include "App/MainScene.h"
#include "JSUtils/Logger.h"

#include <Core/Application.h>
#include <Core/Config.h>
#include <emscripten/emscripten.h>


using namespace TuneZ;

Application *application;

void loop() { application->Render(); }

extern "C" {
int main() {
  auto &config = Config::Get();
  //config.GetItem("test-item");
  // config.Dump();
  application = &Application::Get();
  emscripten_set_main_loop(loop, 0, true);
  emscripten_pause_main_loop();
  return 0;
}

void run() {
  application->Init();
  application->SetScene(CreateRef<MainScene>());
  emscripten_resume_main_loop();
}

void shutdown() {
  emscripten_pause_main_loop();
  Application::Get().Shutdown();
}

void prepare() {

}
}
