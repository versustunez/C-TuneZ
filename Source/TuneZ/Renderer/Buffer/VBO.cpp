#include "VBO.h"

namespace TuneZ {

VBO::VBO() { glGenBuffers(1, &id); }
VBO::~VBO() { remove(); }

VBO::VBO(std::vector<Vertex> &vertices) {
  glGenBuffers(1, &id);
  glBindBuffer(GL_ARRAY_BUFFER, id);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
               vertices.data(), GL_STATIC_DRAW);
}

VBO::VBO(Vector<glm::mat4> &matrixData) {
  glGenBuffers(1, &id);
  glBindBuffer(GL_ARRAY_BUFFER, id);
  glBufferData(GL_ARRAY_BUFFER, matrixData.size() * sizeof(glm::mat4),
               matrixData.data(), GL_DYNAMIC_DRAW);
}

void VBO::bind() const { glBindBuffer(GL_ARRAY_BUFFER, id); }

void VBO::unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }

void VBO::remove() { glDeleteBuffers(1, &id); }

void VBO::setData(Vector<glm::mat4> &matrixData) const {
  glBindBuffer(GL_ARRAY_BUFFER, id);
  glBufferData(GL_ARRAY_BUFFER, matrixData.size() * sizeof(glm::mat4),
               matrixData.data(), GL_DYNAMIC_DRAW);
}

} // namespace TuneZ
