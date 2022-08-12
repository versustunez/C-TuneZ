#pragma once

#include <TypeDefs.h>
#include <GL.h>
#include <glm/glm.hpp>

namespace TuneZ {
// Structure to standardize the vertices used in the meshes
struct Vertex {
  glm::vec3 position{};
  glm::vec3 normal{};
  glm::vec3 color{1.0};
  glm::vec2 texUV{};
  float LightFactor{1.0};
};

class VBO {
public:
  GLuint id{};
  VBO();
  ~VBO();
  explicit VBO(Vector<Vertex> &vertices);
  explicit VBO(Vector<glm::mat4> &matrixData);

  void setData(Vector<glm::mat4>& matrixData) const;
  void bind() const;
  static void unbind();
  void remove();
};
} // namespace TuneZ
