#pragma once
#include "AudioStructs.h"

#include <TypeDefs.h>
namespace TuneZ {
class FIFOBuffer : public Instance<FIFOBuffer> {
public:
  FIFOBuffer() = default;
  ~FIFOBuffer() {
    puts("Delete FIFO");
    delete[] m_data.left;
    delete[] m_data.right;
  };

  void set(float *left, float *right, int size) {
    m_data.left = left;
    m_data.right = right;
    m_data.size = size;
    m_size = size;
    m_hasNewData = true;
  };

  Channel &data() { return m_data; }
  float *left() { return m_data.left; }
  float *right() { return m_data.right; }

  int dataSize() { return m_data.size; }
  int size() { return m_size; }

  void setHasNewData(bool hasNew = true) { m_hasNewData = hasNew; }
  bool hasNewData() { return m_hasNewData; }

protected:
  Channel m_data;
  int m_size{};
  bool m_hasNewData{false};
};

} // namespace TuneZ
