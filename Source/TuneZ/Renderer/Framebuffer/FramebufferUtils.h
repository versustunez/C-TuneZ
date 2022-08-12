#pragma once

#include "FramebufferStructs.h"

#include <GL.h>

namespace TuneZ {
class FramebufferUtils {
public:
  static GLenum textureTarget(bool multiSampled);
  static void createTextures(bool multiSampled, uint32_t *outID,
                             uint32_t count);
  static void bindTexture(bool multiSampled, uint32_t id);
  static void attachColorTexture(uint32_t id, int samples,
                                 GLenum internalFormat, GLenum format,
                                 uint32_t width, uint32_t height, int index,
                                 GLuint typeFormat = GL_UNSIGNED_BYTE);
  static void attachDepthTexture(uint32_t id, int samples, GLenum format,
                                 GLenum attachmentType, uint32_t width,
                                 uint32_t height);
  static bool isDepthFormat(FramebufferTextureFormat format);
  static GLenum FBTextureFormatToGL(FramebufferTextureFormat format);
};
} // namespace TuneZ