#pragma once
#include "AudioStructs.h"

#include <TypeDefs.h>

namespace TuneZ {
// This is a Append at End FIFO Buffer and Remove Previous Data
class FIFOInputBuffer : public Instance<FIFOInputBuffer> {
public:
  FIFOInputBuffer();
  void resize(int size);
  void append(const float* left, const float* right, int size);
  int size() { return m_size; }
  MovingChannel& data() { return m_data; }
protected:
  MovingChannel m_data{};
  int m_size{0};
};
}