#include "MainScene.h"

#include "Audio/FFT.h"
#include "Generator/MeshGenerator.h"
#include <FMT.h>

#include <Scene/Entity.h>
#include <ImGui.h>

namespace TuneZ {

MainScene::MainScene() {
  m_EntityPanel.SetScene(this);
  m_baseShader = Shader::create("/shaders/base.glsl");
  MeshManager::Get().Add("White-Cube", MeshGenerator::createCube(glm::vec3(1.0)));
  MeshManager::Get().Add("White-Sphere", MeshGenerator::createSphere(glm::vec3(1.0), 6));
}

void MainScene::onCustomUpdate(Timestamp ts) {
}
void MainScene::onRenderGUI() {
  m_EntityPanel.OnRender();
}
} // namespace TuneZ
