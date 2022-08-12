#include "PostProcessor.h"

#include "Bloom.h"
#include "Core/Application.h"
#include "Renderer/Renderer.h"

namespace TuneZ {

PostProcessor::PostProcessor() {
  auto &applicationData = Application::applicationData();
  FramebufferSpecification specification;
  specification.width = applicationData.width;
  specification.height = applicationData.height;
  // MainBuffer for Rendering :>
  specification.attachments = {
      FramebufferTextureFormat::RGBA, FramebufferTextureFormat::RGBA,
      FramebufferTextureFormat::RGBA, FramebufferTextureFormat::Depth};
  m_MainFrameBuffer = CreateScope<Framebuffer>(specification);
  // PostProcessingPipelineBuffer
  specification.attachments = {FramebufferTextureFormat::RGBA,
                               FramebufferTextureFormat::RGBA};
  m_MainPostProcessingBuffer = CreateScope<Framebuffer>(specification);
  // Output Buffer
  specification.attachments = {FramebufferTextureFormat::RGBA};
  m_OutBuffer = CreateScope<Framebuffer>(specification);

  m_ToneMappingShader = Shader::create("/shaders/toneMapping.glsl");
  m_LightPass = CreateRef<LightPass>();
}
void PostProcessor::InitPipeline() {
  AddProcessor(CreateRef<BloomProcessor>());
}
void PostProcessor::BeginPipeline() { m_MainFrameBuffer->bind(); }
void PostProcessor::ApplyProcessing() {

  Renderer::DisableDepth();
  m_MainPostProcessingBuffer->bind();
  m_LightPass->Process(m_MainFrameBuffer.get());
  m_MainPostProcessingBuffer->unbind();
  for (auto &processor : m_ProcessingUnits) {
    if (!processor->IsActive)
      continue;
    processor->update(m_MainPostProcessingBuffer.get(), m_OutBuffer.get());
  }
  // Renderer submit FrameBuffer Texture ;)
  m_ToneMappingShader->use();
  Renderer::setShader(m_ToneMappingShader.get());
  Renderer::SubmitQuadTextures({m_OutBuffer->colorAttachmentRenderID(0)});
}
} // namespace TuneZ