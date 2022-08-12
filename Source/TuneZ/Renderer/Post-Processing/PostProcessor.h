#pragma once

#include "LightPass.h"
#include "Renderer/Framebuffer/Framebuffer.h"
#include "Renderer/Shader.h"

#include <TypeDefs.h>

namespace TuneZ {
class PostProcessingUnit {
public:
  bool IsActive{true};
  virtual void update(Framebuffer *framebuffer,
                      Framebuffer *outBuffer) = 0;
};

class PostProcessor {
public:
  PostProcessor();
  // Start the Pipeline and bind FrameBuffer ;)
  void InitPipeline();
  void BeginPipeline();
  void ApplyProcessing();
  void AddProcessor(const Ref<PostProcessingUnit> &unit) {
    m_ProcessingUnits.push_back(unit);
  }
  Ref<LightPass>& GetLightPass() { return m_LightPass; }
protected:
  Scope<Framebuffer> m_MainFrameBuffer;
  Scope<Framebuffer> m_MainPostProcessingBuffer;
  Scope<Framebuffer> m_OutBuffer;
  Vector<Ref<PostProcessingUnit>> m_ProcessingUnits;
  Ref<Shader> m_ToneMappingShader;
  Ref<LightPass> m_LightPass;
};
} // namespace TuneZ
