#pragma once
#include <TypeDefs.h>

namespace TuneZ {
enum class FramebufferTextureFormat
{
  None = 0,
  RGBA8,
  RGBA,
  RED_INTEGER,
  DEPTH24STENCIL8,
  Depth = DEPTH24STENCIL8
};

struct FramebufferTextureSpecification
{
  FramebufferTextureSpecification() = default;
  FramebufferTextureSpecification(FramebufferTextureFormat format)
      : textureFormat(format) {}

  FramebufferTextureFormat textureFormat = FramebufferTextureFormat::None;
};

struct FramebufferAttachmentSpecification
{
  FramebufferAttachmentSpecification() = default;
  FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
      : attachments(attachments) {}

  Vector<FramebufferTextureSpecification> attachments;
};

struct FramebufferSpecification
{
  uint32_t width = 0, height = 0;
  FramebufferAttachmentSpecification attachments;
  uint32_t samples = 1;
  float factor = 1.0;

  bool swapChainTarget = false;
};

} // Namespace TuneZ