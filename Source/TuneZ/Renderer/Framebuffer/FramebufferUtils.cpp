#include "FramebufferUtils.h"

namespace TuneZ {
GLenum FramebufferUtils::textureTarget(bool multiSampled) {
  return multiSampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
}

void FramebufferUtils::createTextures(bool multiSampled, uint32_t *outID,
                                      uint32_t count) {
  glGenTextures(count, outID);
  for (uint32_t i = 0; i < count; ++i) {
    glBindTexture(textureTarget(multiSampled), outID[i]);
  }
}

void FramebufferUtils::bindTexture(bool multiSampled, uint32_t id) {
  glBindTexture(textureTarget(multiSampled), id);
}

void FramebufferUtils::attachColorTexture(uint32_t id, int samples,
                                          GLenum internalFormat, GLenum format,
                                          uint32_t width, uint32_t height,
                                          int index, GLuint typeFormat) {
  bool multiSampled = samples > 1;
  if (multiSampled) {
    printf("Not Implemented\n");
  } else {
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format,
                 typeFormat, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  }
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index,
                         textureTarget(multiSampled), id, 0);
}
void FramebufferUtils::attachDepthTexture(uint32_t id, int samples,
                                          GLenum format, GLenum attachmentType,
                                          uint32_t width, uint32_t height) {
  bool multiSampled = samples > 1;
  if (multiSampled) {
    printf("Not Implemented\n");
  } else {
    glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  }

  glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType,
                         textureTarget(multiSampled), id, 0);
}

bool FramebufferUtils::isDepthFormat(FramebufferTextureFormat format) {
  switch (format) {
  case FramebufferTextureFormat::DEPTH24STENCIL8: return true;
  default: return false;
  }
}

GLenum FramebufferUtils::FBTextureFormatToGL(FramebufferTextureFormat format) {
  switch (format) {
  case FramebufferTextureFormat::RGBA8: return GL_RGBA8;
  case FramebufferTextureFormat::RED_INTEGER: return GL_RED_INTEGER;
  default: return 0;
  }
}
} // namespace TuneZ