#pragma once
#include "PostProcessor.h"
#include "Renderer/Shader.h"
namespace TuneZ {

struct BloomPass {
  explicit BloomPass(FramebufferSpecification specification);
  void doPass(GLuint textureID, Shader *pShader);
  GLuint GetTexture() { return m_FrameBuffer[1]->colorAttachmentRenderID(0); }
  Ref<Framebuffer> m_FrameBuffer[2];
};
class BloomProcessor : public PostProcessingUnit {
public:
  BloomProcessor();
  void update(Framebuffer *framebuffer, Framebuffer *outBuffer) override;

public:
  Vector<BloomPass> m_BloomPass{};
  Ref<Shader> m_BloomShader;
  Ref<Shader> m_CombineShader;
  size_t m_LastPingPong = 0;
};
} // namespace TuneZ