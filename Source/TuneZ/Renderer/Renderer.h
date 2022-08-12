#pragma once
#include "Camera.h"
#include "Mesh.h"
#include "Scene/Components.h"
#include "Shader.h"

#include <GLM.h>
#include <TypeDefs.h>
namespace TuneZ {
class InstanceRenderer;
class LightPass;
class Renderer {
public:
  static void init();
  static void clearColor(glm::vec4 color);
  static void clear();
  static void viewPort(int x, int y, int w, int h);
  static void beginScene(const Ref<Camera> &camera);
  static void DisableDepth();
  static void bindTextureShader();
  static void setShader(Shader* shader) { s_SceneData->ActiveShader = shader; }
  static void SubmitQuadTextures(const Vector<uint32_t>& textures);
protected:
  struct SceneData {
    glm::mat4 Matrix;
    glm::vec3 LightPosition{0,100,10};
    glm::vec3 CameraPosition{0};
    Ref<Shader> textureShader;
    Shader* ActiveShader;
    Scope<Mesh> quadMesh;
  };
  static Scope<SceneData> s_SceneData;
  static void createQuadMesh();
  friend InstanceRenderer;
  friend LightPass;
};
} // namespace TuneZ