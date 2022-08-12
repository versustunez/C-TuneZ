#pragma once

#include "Renderer/Buffer/VAO.h"
#include "Renderer/Buffer/EBO.h"

#include <Core/Asset.h>
#include <Renderer/Buffer/VBO.h>
#include <TypeDefs.h>

namespace TuneZ {
class MeshUtils {
public:
  static Vector<Vertex> createVertices(float* vertices, int count);
  static void fillNormals(Vector<Vertex>& vertex, float* normals);
  static void fillColor(Vector<Vertex>& vertex, float* color);
  static void fillSingleColor(Vector<Vertex>& vertex, glm::vec3 color);
  static void fillUV(Vector<Vertex>& vertex, float* uv);
  static void calcNormals(Vector<Vertex>& vertex);
};
class Mesh : public Asset {
public:
  Mesh(Vector<Vertex>& vertices, Vector<GLuint>& indices);
  void bind() { vao.Bind(); }
  void unbind() { vao.Unbind(); }
  unsigned long size() const { return indices.size(); }
  void setData(Vector<glm::mat4>& instanceMatrix) const;
  glm::mat4 transformation;
public:
  VAO vao{};
  Ref<VBO> vbo{};
  Ref<EBO> ebo;
  Ref<VBO> instanceVBO;
protected:
  Vector<Vertex> vertices;
  Vector<GLuint> indices;
};

} // namespace TuneZ