#pragma once
#include "VBO.h"

#include <GL.h>

namespace TuneZ {
class VAO {
public:
  GLuint id{};
  VAO();
  ~VAO();
  void LinkAttrib(VBO &VBO, GLuint layout, GLuint numComponents, GLenum type,
                  GLsizeiptr stride, void *offset);
  void Bind() const;
  static void Unbind();
  void Remove();
};
} // namespace TuneZ