#include "LightPass.h"

#include "Renderer/Renderer.h"

#include <FMT.h>
#include <algorithm>

namespace TuneZ {
LightPass::LightPass() {
  m_LightPassShader = Shader::create("/shaders/lightPass.glsl");
}
void LightPass::Process(Framebuffer *data) {
  m_LightPassShader->use();
  Renderer::setShader(m_LightPassShader.get());
  m_LightPassShader->setFloat3("u_ViewPos",
                               Renderer::s_SceneData->CameraPosition);
  // Upload Lights ;)
  size_t lights = m_Lights.size() > 32 ? 32 : m_Lights.size();
  m_LightPassShader->setInt("u_LightCount", (int)lights);
  for (size_t i = 0; i < lights; i++) {
    auto &light = m_Lights[i];
    m_LightPassShader->setFloat3(fmt::format("u_Lights[{}].Position", i),
                                 light.Position);
    m_LightPassShader->setFloat3(fmt::format("u_Lights[{}].Color", i),
                                 light.Color);
    m_LightPassShader->setFloat4(fmt::format("u_Lights[{}].Meta", i),
                                light.MetaData);
  }
  Renderer::SubmitQuadTextures({
      data->colorAttachmentRenderID(0),
      data->colorAttachmentRenderID(1),
      data->colorAttachmentRenderID(2),
      data->depthAttachment(),
  });
}
void LightPass::SetLights(const Vector<LightComponent> &vector) {
  m_Lights = vector;
  if (vector.size() > 32) {
    glm::vec3 cameraPos = Renderer::s_SceneData->CameraPosition;
    std::sort(m_Lights.begin(), m_Lights.end(),
              [cameraPos](TuneZ::LightComponent &a, TuneZ::LightComponent &b) {
                return glm::distance(a.Position, cameraPos) >
                       glm::distance(b.Position, cameraPos);
              });
  }
}

} // namespace TuneZ