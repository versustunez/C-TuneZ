#include "Framebuffer.h"

#include "FramebufferUtils.h"
#include "JSUtils/Logger.h"

#include <GL.h>

namespace TuneZ {
static const uint32_t s_MaxFramebufferSize = 8192;
Framebuffer::Framebuffer(const FramebufferSpecification &specification)
    : m_specification(specification) {
  for (auto spec : m_specification.attachments.attachments) {
    if (!FramebufferUtils::isDepthFormat(spec.textureFormat))
      m_colorAttachmentSpecifications.emplace_back(spec);
    else
      m_depthAttachmentSpecification = spec;
  }

  invalidate();
}

Framebuffer::~Framebuffer() {
  glDeleteFramebuffers(1, &m_id);
  glDeleteTextures(m_colorAttachments.size(), m_colorAttachments.data());
  glDeleteTextures(1, &m_depthAttachment);
}

void Framebuffer::invalidate() {
  if (m_id) {
    glDeleteFramebuffers(1, &m_id);
    glDeleteTextures(m_colorAttachments.size(), m_colorAttachments.data());
    glDeleteTextures(1, &m_depthAttachment);

    m_colorAttachments.clear();
    m_depthAttachment = 0;
  }

  glGenFramebuffers(1, &m_id);
  glBindFramebuffer(GL_FRAMEBUFFER, m_id);

  bool multisample = m_specification.samples > 1;
  uint32_t width = m_specification.width;
  uint32_t height = m_specification.height;

  if (m_colorAttachmentSpecifications.size()) {
    m_colorAttachments.resize(m_colorAttachmentSpecifications.size());
    FramebufferUtils::createTextures(multisample, m_colorAttachments.data(),
                                     m_colorAttachments.size());
    for (size_t i = 0; i < m_colorAttachments.size(); i++) {
      FramebufferUtils::bindTexture(multisample, m_colorAttachments[i]);
      switch (m_colorAttachmentSpecifications[i].textureFormat) {
      case FramebufferTextureFormat::RGBA8:
        FramebufferUtils::attachColorTexture(m_colorAttachments[i],
                                             m_specification.samples, GL_RGBA8,
                                             GL_RGBA, width, height, i);
        break;
      case FramebufferTextureFormat::RGBA:
        FramebufferUtils::attachColorTexture(
            m_colorAttachments[i], m_specification.samples, GL_RGBA16F, GL_RGBA,
            width, height, i, GL_FLOAT);
        break;
      case FramebufferTextureFormat::RED_INTEGER:
        FramebufferUtils::attachColorTexture(m_colorAttachments[i],
                                             m_specification.samples, GL_R32I,
                                             GL_RED_INTEGER, width, height, i);
        break;
      case FramebufferTextureFormat::None:
      case FramebufferTextureFormat::DEPTH24STENCIL8: break;
      }
    }
  }

  if (m_depthAttachmentSpecification.textureFormat !=
      FramebufferTextureFormat::None) {
    FramebufferUtils::createTextures(multisample, &m_depthAttachment, 1);
    FramebufferUtils::bindTexture(multisample, m_depthAttachment);
    switch (m_depthAttachmentSpecification.textureFormat) {
    case FramebufferTextureFormat::DEPTH24STENCIL8:
      FramebufferUtils::attachDepthTexture(
          m_depthAttachment, m_specification.samples, GL_DEPTH24_STENCIL8,
          GL_DEPTH_STENCIL_ATTACHMENT, width, height);
      break;
    default: break;
    }
  }
  if (m_colorAttachments.size() > 1) {
    GLenum buffers[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
                         GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
    glDrawBuffers(m_colorAttachments.size(), buffers);
  } else if (m_colorAttachments.empty()) {
    glDrawBuffers(1, GL_NONE);
  }

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    ERROR("Framebuffer {} is incomplete: {}", m_id, m_colorAttachments.size());
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::bind() {
  glBindFramebuffer(GL_FRAMEBUFFER, m_id);
  glViewport(0, 0, m_specification.width, m_specification.height);
  Vector<GLuint> drawBuffers = {};
  drawBuffers.resize(m_colorAttachments.size());
  for (int i = 0; i < drawBuffers.size(); i++) {
    drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
  }
  if (!drawBuffers.empty()) {
    glDrawBuffers(drawBuffers.size(), drawBuffers.data());
  } else {
    glDrawBuffers(1, GL_NONE);
  }
}

void Framebuffer::unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
void Framebuffer::resize(uint32_t width, uint32_t height) {
  if (width == 0 || height == 0 || width > s_MaxFramebufferSize ||
      height > s_MaxFramebufferSize) {
    return;
  }
  m_specification.width = width;
  m_specification.height = height;
  LOG("FB Size: {}, {}", width, height);
  invalidate();
}

int Framebuffer::readPixel(uint32_t attachmentIndex, int x, int y) {
  glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
  int pixelData;
  glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
  return pixelData;
}

Ref<Framebuffer> Framebuffer::create(const FramebufferSpecification &spec) {
  return CreateRef<Framebuffer>(spec);
}
} // namespace TuneZ