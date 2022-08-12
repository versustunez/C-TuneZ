#pragma once

#include <GL.h>
#include <TypeDefs.h>
#include <vector>

namespace TuneZ {
class EBO {
public:
  GLuint id{};
  explicit EBO(Vector<GLuint> &indices);
  ~EBO();
  void Bind();
  static void Unbind();
  void Remove();
};
} // namespace TuneZ