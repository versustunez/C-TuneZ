#include "Renderer.h"

#include "InstanceRenderer.h"
#include "JSUtils/Logger.h"
#include "Scene/Components.h"

#include <GL.h>
#include <TypeDefs.h>
namespace TuneZ {
void OpenGLMessageCallback(unsigned source, unsigned type, unsigned id,
                           unsigned severity, int length, const char *message,
                           const void *userParam) {
  switch (severity) {
  case GL_DEBUG_SEVERITY_HIGH: ERROR(message); return;
  case GL_DEBUG_SEVERITY_MEDIUM: ERROR(message); return;
  case GL_DEBUG_SEVERITY_LOW: WARN(message); return;
  case GL_DEBUG_SEVERITY_NOTIFICATION: DBG(message); return;
  }
}

Scope<Renderer::SceneData> Renderer::s_SceneData =
    CreateScope<Renderer::SceneData>();
void Renderer::clearColor(glm::vec4 color) {
  glClearColor(color.r, color.g, color.b, color.a);
}
void Renderer::viewPort(int x, int y, int w, int h) { glViewport(x, y, w, h); }
void Renderer::clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }
void Renderer::beginScene(const Ref<Camera> &camera) {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  s_SceneData->Matrix = camera->projection() * camera->view();
  s_SceneData->CameraPosition = camera->position();
}

void Renderer::DisableDepth() {
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
}

void Renderer::init() {
#if DEBUG
  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDebugMessageCallback(OpenGLMessageCallback, nullptr);

  glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE,
                        GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  // Create Quad Mesh and Texture Mesh
  createQuadMesh();
}

void Renderer::createQuadMesh() {
  float vertices[4 * 3] = {-1.0, -1.0, 1.0f, 1.0,  -1.0, 1.0f,
                           1.0,  1.0,  1.0f, -1.0, 1.0,  1.0f};
  Vector<GLuint> indices = {0, 1, 2, 0, 2, 3};
  float uvs[4 * 2] = {0, 0, 1, 0, 1, 1, 0, 1};
  auto quadVertices = MeshUtils::createVertices(vertices, 4 * 3);
  MeshUtils::fillUV(quadVertices, uvs);
  s_SceneData->quadMesh = CreateScope<Mesh>(quadVertices, indices);
  s_SceneData->textureShader = Shader::create("/shaders/texture.glsl");
}
void Renderer::bindTextureShader() {
  s_SceneData->textureShader->use();
  s_SceneData->ActiveShader = s_SceneData->textureShader.get();
}

void Renderer::SubmitQuadTextures(const Vector<uint32_t> &textures) {
  for (int i = 0; i < textures.size(); ++i) {
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, textures[i]);
    s_SceneData->ActiveShader->setInt(
        fmt::format("u_Texture{}", i == 0 ? "" : std::to_string(i)), i);
  }
  auto *mesh = s_SceneData->quadMesh.get();
  mesh->bind();
  glDrawElements(GL_TRIANGLES, mesh->size(), GL_UNSIGNED_INT, nullptr);
}

} // namespace TuneZ