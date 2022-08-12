#pragma once

#include "Renderer/Framebuffer/Framebuffer.h"
#include "Renderer/Shader.h"
#include "Scene/Components.h"
namespace TuneZ {

class LightPass {
public:
  LightPass();
  void Process(Framebuffer *data);
  void SetLights(const Vector<LightComponent>& vector);
protected:
  Ref<Shader> m_LightPassShader;
  Vector<LightComponent> m_Lights;
};

} // namespace TuneZ
