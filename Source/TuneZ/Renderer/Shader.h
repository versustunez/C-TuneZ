#pragma once

#include <Core/Asset.h>
#include <GL.h>
#include <GLM.h>
#include <TypeDefs.h>

namespace TuneZ {
struct ShaderCompileStatus {
  uint32_t shader;
  bool isFailed;
};
class Shader : public Asset {
public:
  static Ref<Shader> create(const char *name, const char *vertexSrc,const char *fragmentSrc);
  static Ref<Shader> create(const char *file);
  static Ref<Shader> create();

public:
  ~Shader();
  void fromFile(const char *filename);
  void init(const char *name, const char *vertexSrc, const char *fragmentSrc);

  std::string &name() { return m_name; }
  uint32_t id() const { return m_id; }
  bool isInvalid() { return m_isErrored; }

  void use();

  void setInt(const std::string &name, int value);
  void setIntArray(const std::string &name, int *values, uint32_t count);
  void setFloat(const std::string &name, float value);
  void setFloat2(const std::string &name, const glm::vec2 &value);
  void setFloat3(const std::string &name, const glm::vec3 &value);
  void setFloat4(const std::string &name, const glm::vec4 &value);
  void setMat3(const std::string &name, const glm::mat3 &value);
  void setMat4(const std::string &name, const glm::mat4 &value);

  static void reloadShader(uint32_t handle);

protected:
  static Map<GLenum, std::string> processFile(const String &source);
  static GLenum shaderTypeFromString(const std::string &type);
  static ShaderCompileStatus compileShader(const char *source, GLenum type);
  void fetchUniforms();

protected:
  uint32_t m_id{};
  bool m_isErrored{false};
  std::string m_name;
  Map<std::string, GLint> m_uniforms;
};

class ShaderLib : public Instance<ShaderLib> {
public:
  void addShader(Ref<Shader> &shader);
  bool hasShader(const std::string &name);
  void removeShader(const std::string &name);
  Shader *getShader(const std::string &name);
  Ref<Shader> getSharedShader(const std::string &name);
  Map<std::string, Ref<Shader>>& shaders() { return m_shaders;}

protected:
  Map<std::string, Ref<Shader>> m_shaders;
};
} // namespace TuneZ