#pragma once

#include "Core/Asset.h"
#include <GLM.h>

namespace TuneZ {
class MeshGenerator {
public:
  static AssetHandle createSphere(glm::vec3 color, int divisions = 2);
  static AssetHandle createCube(glm::vec3 color);
};

}

