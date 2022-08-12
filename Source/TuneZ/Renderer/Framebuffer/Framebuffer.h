#pragma once

#include "FramebufferStructs.h"

namespace TuneZ {
class Framebuffer {
public:
  Framebuffer(const FramebufferSpecification &spec);
  ~Framebuffer();
  void invalidate();
  void bind();
  void unbind();
  void resize(uint32_t width, uint32_t height);
  int readPixel(uint32_t attachmentIndex, int x, int y);

  uint32_t colorAttachmentRenderID(uint32_t index = 0) {
    return m_colorAttachments[index];
  }

  uint32_t depthAttachment(uint32_t index = 0) const {
    return m_depthAttachment;
  }

  const FramebufferSpecification specification() { return m_specification; }
  static Ref<Framebuffer> create(const FramebufferSpecification &spec);

private:
  uint32_t m_id{0};
  FramebufferSpecification m_specification{};
  Vector<FramebufferTextureSpecification> m_colorAttachmentSpecifications{};
  FramebufferTextureSpecification m_depthAttachmentSpecification{
      FramebufferTextureFormat::None};
  Vector<uint32_t> m_colorAttachments{};
  uint32_t m_depthAttachment{0};
};
} // namespace TuneZ