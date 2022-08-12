#include "Mesh.h"

#include "Renderer/Buffer/EBO.h"

namespace TuneZ {
Mesh::Mesh(Vector<Vertex> &_vertices, Vector<GLuint> &_indices)
    : vertices(_vertices),
      indices(_indices) {

  vao.Bind();
  vbo = CreateRef<VBO>(vertices);
  ebo = CreateRef<EBO>(indices);
  instanceVBO = CreateRef<VBO>();
  vao.LinkAttrib(*vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void *)nullptr);
  vao.LinkAttrib(*vbo, 1, 3, GL_FLOAT, sizeof(Vertex),
                 (void *)(3 * sizeof(float)));
  vao.LinkAttrib(*vbo, 2, 3, GL_FLOAT, sizeof(Vertex),
                 (void *)(6 * sizeof(float)));
  vao.LinkAttrib(*vbo, 3, 2, GL_FLOAT, sizeof(Vertex),
                 (void *)(9 * sizeof(float)));
  // TRANSFORM
  {
    instanceVBO->bind();
    vao.LinkAttrib(*instanceVBO, 4, 4, GL_FLOAT, sizeof(glm::mat4),
                   (void *)nullptr);
    vao.LinkAttrib(*instanceVBO, 5, 4, GL_FLOAT, sizeof(glm::mat4),
                   (void *)(1 * sizeof(glm::vec4)));
    vao.LinkAttrib(*instanceVBO, 6, 4, GL_FLOAT, sizeof(glm::mat4),
                   (void *)(2 * sizeof(glm::vec4)));
    vao.LinkAttrib(*instanceVBO, 7, 4, GL_FLOAT, sizeof(glm::mat4),
                   (void *)(3 * sizeof(glm::vec4)));
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);
    glVertexAttribDivisor(7, 1);
  }
  TuneZ::VAO::Unbind();
  ebo->Unbind();
  VBO::unbind();
}

void Mesh::setData(Vector<glm::mat4> &instanceMatrix) const {
  instanceVBO->bind();
  instanceVBO->setData(instanceMatrix);
  VBO::unbind();
}

Vector<Vertex> MeshUtils::createVertices(float *vertices, int count) {
  Vector<Vertex> vertex;
  for (size_t i = 0; i < count; i += 3) {
    vertex.push_back(
        {{vertices[i], vertices[i + 1], vertices[i + 2]}, {}, {}, {}});
  }
  return vertex;
}
void MeshUtils::fillNormals(Vector<Vertex> &vertex, float *normals) {
  size_t i = 0;
  for (auto &item : vertex) {
    item.normal = {normals[i], normals[i + 1], normals[i + 2]};
    i += 3;
  }
}
void MeshUtils::fillColor(Vector<Vertex> &vertex, float *color) {
  size_t i = 0;
  for (auto &item : vertex) {
    item.color = {color[i], color[i + 1], color[i + 2]};
    i += 3;
  }
}
void MeshUtils::fillSingleColor(Vector<Vertex> &vertex, glm::vec3 color) {
  for (auto &item : vertex) {
    item.color = color;
  }
}
void MeshUtils::fillUV(Vector<Vertex> &vertex, float *uv) {
  size_t i = 0;
  for (auto &item : vertex) {
    item.texUV = {uv[i], uv[i + 1]};
    i += 2;
  }
}
// Stupid Function XD
void MeshUtils::calcNormals(Vector<Vertex> &vertex) {
  for (auto &item : vertex) {
    // @TODO: add real logic for this... this only normalized them but that is
    // wrong
    item.normal = glm::normalize(item.position);
  }
}
} // namespace TuneZ