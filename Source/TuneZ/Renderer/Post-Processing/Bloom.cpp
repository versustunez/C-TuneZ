#include "Bloom.h"

#include "Core/Application.h"
#include "Renderer/Renderer.h"

namespace TuneZ {

static int nextPowerOfTwo (float value)
{
  auto v = (unsigned int) value;
  v--;
  v |= v >> 1;
  v |= v >> 2;
  v |= v >> 4;
  v |= v >> 8;
  v |= v >> 16;
  v++;
  return v;
}
BloomProcessor::BloomProcessor() {
  auto &appData = Application::applicationData();
  FramebufferSpecification specification;
  float aspectRatio = appData.width / appData.height;
  specification.width = nextPowerOfTwo(appData.width)/2;
  specification.height = specification.width*aspectRatio;
  specification.attachments = {FramebufferTextureFormat::RGBA};

  for (int i = 0; i < 4; ++i) {
    specification.width *= 0.75;
    specification.height *= 0.75;
    m_BloomPass.push_back(BloomPass{specification});
  }
  m_BloomShader = Shader::create("/shaders/bloom.glsl");
  m_CombineShader = Shader::create("/shaders/bloomCombine.glsl");
}
void BloomProcessor::update(Framebuffer *framebuffer, Framebuffer *outBuffer) {

  m_BloomShader->use();
  bool firstIteration = true;
  for (int i = 0; i < m_BloomPass.size(); ++i) {
    m_BloomPass[i].doPass(firstIteration
                              ? framebuffer->colorAttachmentRenderID(1)
                              : m_BloomPass[i - 1].GetTexture(),
                          m_BloomShader.get());
    firstIteration = false;
  }
  m_CombineShader->use();
  Renderer::setShader(m_CombineShader.get());
  m_CombineShader->setInt("u_Passes", m_BloomPass.size());
  Vector<GLuint> textures{};
  textures.push_back(framebuffer->colorAttachmentRenderID(0));
  for (auto & m_BloomPas : m_BloomPass) {
    textures.push_back(m_BloomPas.GetTexture());
  }
  outBuffer->bind();
  Renderer::SubmitQuadTextures(textures);
  outBuffer->unbind();
}

BloomPass::BloomPass(FramebufferSpecification specification) {
  for (auto &buffer : m_FrameBuffer) {
    buffer = Framebuffer::create(specification);
  }
}

void BloomPass::doPass(GLuint textureID, Shader *pShader) {
  bool isHorizontal = true;
  for (auto &buffer : m_FrameBuffer) {
    buffer->bind();
    Renderer::SubmitQuadTextures({textureID});
    textureID = buffer->colorAttachmentRenderID(0);
    pShader->setInt("u_Horizontal", isHorizontal);
    isHorizontal = false;
  }
}

} // namespace TuneZ
