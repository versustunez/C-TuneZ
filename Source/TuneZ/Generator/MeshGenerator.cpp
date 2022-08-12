#include "MeshGenerator.h"

#include "Renderer/Mesh.h"

namespace TuneZ {
namespace CubeToSphere {
static const glm::vec3 origins[6] = {
    glm::vec3(-1.0, -1.0, -1.0), glm::vec3(1.0, -1.0, -1.0),
    glm::vec3(1.0, -1.0, 1.0),   glm::vec3(-1.0, -1.0, 1.0),
    glm::vec3(-1.0, 1.0, -1.0),  glm::vec3(-1.0, -1.0, 1.0)};
static const glm::vec3 rights[6] = {
    glm::vec3(2.0, 0.0, 0.0),  glm::vec3(0.0, 0.0, 2.0),
    glm::vec3(-2.0, 0.0, 0.0), glm::vec3(0.0, 0.0, -2.0),
    glm::vec3(2.0, 0.0, 0.0),  glm::vec3(2.0, 0.0, 0.0)};
static const glm::vec3 ups[6] = {
    glm::vec3(0.0, 2.0, 0.0), glm::vec3(0.0, 2.0, 0.0),
    glm::vec3(0.0, 2.0, 0.0), glm::vec3(0.0, 2.0, 0.0),
    glm::vec3(0.0, 0.0, 2.0), glm::vec3(0.0, 0.0, -2.0)};
}; // namespace CubeToSphere

AssetHandle MeshGenerator::createSphere(glm::vec3 color, int divisions) {
  const double step = 1.0 / double(divisions);
  const glm::vec3 step3(step, step, step);

  Vector<float> vertices;

  for (uint32_t face = 0; face < 6; ++face)
  {
    const glm::vec3 origin = CubeToSphere::origins[face];
    const glm::vec3 right = CubeToSphere::rights[face];
    const glm::vec3 up = CubeToSphere::ups[face];
    for (uint32_t j = 0; j < divisions + 1; ++j)
    {
      const glm::vec3 j3(j, j, j);
      for (uint32_t i = 0; i < divisions + 1; ++i)
      {
        const glm::vec3 i3(i, i, i);
        const glm::vec3 p = origin + step3 * (i3 * right + j3 * up);
        const glm::vec3 p2 = p * p;
        vertices.emplace_back(p.x * std::sqrt(1.0 - 0.5 * (p2.y + p2.z) + p2.y*p2.z / 3.0));
        vertices.emplace_back(p.y * std::sqrt(1.0 - 0.5 * (p2.z + p2.x) + p2.z*p2.x / 3.0));
        vertices.emplace_back(p.z * std::sqrt(1.0 - 0.5 * (p2.x + p2.y) + p2.x*p2.y / 3.0));
      }
    }
  }

  Vector<GLuint> indices;
  auto addQuadAlt = [&](uint32_t a, uint32_t b, uint32_t c, uint32_t d) {
    indices.emplace_back(a);
    indices.emplace_back(b);
    indices.emplace_back(c);
    indices.emplace_back(a);
    indices.emplace_back(c);
    indices.emplace_back(d);
  };
  auto addQuad = [&](uint32_t a, uint32_t b, uint32_t c, uint32_t d) {
    indices.emplace_back(a);
    indices.emplace_back(b);
    indices.emplace_back(d);
    indices.emplace_back(b);
    indices.emplace_back(c);
    indices.emplace_back(d);
  };
  const uint32_t k = divisions + 1;
  for (uint32_t face = 0; face < 6; ++face)
  {
    for (uint32_t j = 0; j < divisions; ++j)
    {
      const bool bottom = j < (divisions / 2);
      for (uint32_t i = 0; i < divisions; ++i)
      {
        const bool left = i < (divisions / 2);
        const uint32_t a = (face * k + j) * k + i;
        const uint32_t b = (face * k + j) * k + i + 1;
        const uint32_t c = (face * k + j + 1) * k + i;
        const uint32_t d = (face * k + j + 1) * k + i + 1;
        if (bottom ^ left) addQuadAlt(a, c, d, b);
        else addQuad(a, c, d, b);
      }
    }
  }
  auto &manager = AssetManager::Get();
  auto vertexData = MeshUtils::createVertices(vertices.data(), vertices.size());
  MeshUtils::fillSingleColor(vertexData, color);
  MeshUtils::calcNormals(vertexData);
  auto *mesh = manager.create<Mesh>(vertexData, indices);
  return mesh->handle();
}
AssetHandle MeshGenerator::createCube(glm::vec3 color) {
  auto &manager = AssetManager::Get();
  float rawVertices[3 * 8] = {-0.5, -0.5, 0.5, 0.5, -0.5, 0.5,  0.5,  0.5,
                              0.5,  -0.5, 0.5, 0.5, -0.5, -0.5, -0.5, 0.5,
                              -0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5,  -0.5};
  auto vertices = MeshUtils::createVertices(rawVertices, 24);
  MeshUtils::fillSingleColor(vertices, color);
  MeshUtils::calcNormals(vertices);
  Vector<GLuint> indices = {0, 1, 2, 2, 3, 0, 1, 5, 6, 6, 2, 1,
                            7, 6, 5, 5, 4, 7, 4, 0, 3, 3, 7, 4,
                            4, 5, 1, 1, 0, 4, 3, 2, 6, 6, 7, 3};
  auto *mesh = manager.create<Mesh>(vertices, indices);
  return mesh->handle();
}
} // namespace TuneZ