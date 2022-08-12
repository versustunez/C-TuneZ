#include "Shader.h"

#include "JSUtils/Logger.h"

#include <Utils/FileHandler.h>
#include <cstring>

namespace TuneZ {
GLenum Shader::shaderTypeFromString(const std::string &type) {
  if (type == "vertex")
    return GL_VERTEX_SHADER;
  if (type == "fragment" || type == "pixel")
    return GL_FRAGMENT_SHADER;
  return 0;
}
Shader::~Shader() {
  if (m_id > 0)
    glDeleteProgram(m_id);
}

void Shader::init(const char *name, const char *vertexSrc,
                  const char *fragmentSrc) {
  m_name = name;
  if (m_id > 0)
    glDeleteProgram(m_id);
  auto vertex = compileShader(vertexSrc, GL_VERTEX_SHADER);
  auto fragment = compileShader(fragmentSrc, GL_FRAGMENT_SHADER);

  if (vertex.isFailed || fragment.isFailed) {
    m_isErrored = true;
    return;
  }
  m_id = glCreateProgram();
  glAttachShader(m_id, vertex.shader);
  glAttachShader(m_id, fragment.shader);
  glLinkProgram(m_id);
  int success;
  glGetProgramiv(m_id, GL_LINK_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetProgramInfoLog(m_id, 512, nullptr, infoLog);
    ERROR(infoLog);
    m_isErrored = true;
  }
  glDeleteShader(vertex.shader);
  glDeleteShader(fragment.shader);
  fetchUniforms();
}

void Shader::fromFile(const char *filename) {
  auto fileContent = FileHandler::readFile(filename);
  auto shaderSources = processFile(fileContent);
  auto name = FileHandler::getFileName(filename);
  m_file = filename;
  init(name.c_str(), shaderSources[GL_VERTEX_SHADER].c_str(),
       shaderSources[GL_FRAGMENT_SHADER].c_str());
}
Ref<Shader> Shader::create() {
  auto shader = CreateRef<Shader>();
  AssetManager::Get().add(shader);
  return shader;
}
Ref<Shader> Shader::create(const char *file) {
  auto shader = create();
  shader->fromFile(file);
  return shader;
}
Ref<Shader> Shader::create(const char *name, const char *vertexSrc,
                           const char *fragmentSrc) {
  auto shader = create();
  shader->init(name, vertexSrc, fragmentSrc);
  return shader;
}

void Shader::use() { glUseProgram(m_id); }

void Shader::setInt(const std::string &name, int value) {
  glUniform1i(m_uniforms[name], value);
}

void Shader::setIntArray(const std::string &name, int *values, uint32_t count) {
  glUniform1iv(m_uniforms[name], count, values);
}

void Shader::setFloat(const std::string &name, float value) {
  glUniform1f(m_uniforms[name], value);
}

void Shader::setFloat2(const std::string &name, const glm::vec2 &value) {
  glUniform2f(m_uniforms[name], value.x, value.y);
}

void Shader::setFloat3(const std::string &name, const glm::vec3 &value) {
  glUniform3f(m_uniforms[name], value.x, value.y, value.z);
}
void Shader::setFloat4(const std::string &name, const glm::vec4 &value) {
  glUniform4f(m_uniforms[name], value.x, value.y, value.z, value.w);
}

void Shader::setMat3(const std::string &name, const glm::mat3 &value) {
  glUniformMatrix4fv(m_uniforms[name], 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setMat4(const std::string &name, const glm::mat4 &value) {
  glUniformMatrix4fv(m_uniforms[name], 1, GL_FALSE, glm::value_ptr(value));
}

Map<GLenum, std::string> Shader::processFile(const String &rawString) {
  Map<GLenum, std::string> shaderSources{};
  auto &source = rawString;
  const char *typeToken = "#type";
  size_t typeTokenLength = strlen(typeToken);
  size_t pos = source.find(typeToken, 0);
  while (pos != std::string::npos) {
    size_t eol = source.find_first_of('\n', pos);
    size_t begin = pos + typeTokenLength + 1;
    std::string type = source.substr(begin, eol - begin);
    size_t nextLinePos = source.find_first_not_of("\n", eol);
    pos = source.find(typeToken, nextLinePos);
    shaderSources[shaderTypeFromString(type)] =
        (pos == std::string::npos) ? source.substr(nextLinePos)
                              : source.substr(nextLinePos, pos - nextLinePos);
  }
  return shaderSources;
}
ShaderCompileStatus Shader::compileShader(const char *source, GLenum type) {
  auto shader = glCreateShader(type);

  int success;
  glShaderSource(shader, 1, &source, 0);
  glCompileShader(shader);
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    int maxLength;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
    char infoLog[maxLength];
    glGetShaderInfoLog(shader, maxLength, nullptr, infoLog);
    ERROR("{}\n{}", type == GL_FRAGMENT_SHADER ? "fragment" : "vertex",
        std::string(infoLog));
    DBG("[SOURCE]\n\n{}", source);
    return {shader, true};
  }
  return {shader, false};
}

void Shader::fetchUniforms() {
  if (m_isErrored)
    return;
  int count;
  int activeAttribs;
  GLint size;
  GLenum type;

  const GLsizei bufSize = 64;
  GLchar name[bufSize];
  GLsizei length;
  glGetProgramiv(m_id, GL_ACTIVE_UNIFORMS, &count);
  glGetProgramiv(m_id, GL_ACTIVE_ATTRIBUTES, &activeAttribs);
  DBG("{} Active Attribs: {}", m_name, activeAttribs);
  DBG("{} Active Uniforms: {}", m_name, count);
  for (int i = 0; i < count; ++i) {
    glGetActiveUniform(m_id, (GLuint)i, bufSize, &length, &size, &type, name);
    DBG("{} Uniform > {}", m_name, name);
    m_uniforms[name] = glGetUniformLocation(m_id, name);
  }
}
void Shader::reloadShader(uint32_t handle) {
  Shader *oldShader = (Shader *)AssetManager::Get().assetRef(handle).get();
  oldShader->fromFile(oldShader->file().c_str());
}

void ShaderLib::addShader(Ref<Shader> &shader) {
  m_shaders[shader->name()] = shader;
}

bool ShaderLib::hasShader(const std::string &name) {
  return m_shaders.find(name) != m_shaders.end();
}

void ShaderLib::removeShader(const std::string &name) {
  if (m_shaders.find(name) != m_shaders.end()) {
    m_shaders.erase(name);
  }
}

Shader *ShaderLib::getShader(const std::string &name) {
  if (m_shaders.find(name) != m_shaders.end()) {
    return m_shaders[name].get();
  }
  return nullptr;
}

Ref<Shader> ShaderLib::getSharedShader(const std::string &name) {
  return m_shaders[name];
}
} // namespace TuneZ
