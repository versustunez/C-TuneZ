#include "VAO.h"

namespace TuneZ {
VAO::VAO() {
  glGenVertexArrays(1, &id);
}

VAO::~VAO() {
  Remove();
}

void VAO::LinkAttrib(VBO &VBO, GLuint layout, GLuint numComponents, GLenum type,
                     GLsizeiptr stride, void *offset) {
  VBO.bind();
  glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
  glEnableVertexAttribArray(layout);
  TuneZ::VBO::unbind();
}

// Binds the VAO
void VAO::Bind() const {
  glBindVertexArray(id);
}

// Unbinds the VAO
void VAO::Unbind() { glBindVertexArray(0); }

// Deletes the VAO
void VAO::Remove() { glDeleteVertexArrays(1, &id); }
} // namespace TuneZ