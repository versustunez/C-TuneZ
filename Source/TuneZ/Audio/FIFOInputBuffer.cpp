#include "FIFOInputBuffer.h"

#define DEFAULT_BUFFER_SIZE 8192

namespace TuneZ {

FIFOInputBuffer::FIFOInputBuffer() {
  resize(DEFAULT_BUFFER_SIZE);
}

void FIFOInputBuffer::resize(int size) {
  m_data.left.resize(size);
  m_data.right.resize(size);
  m_size = size;
  m_data.size = size;
}
void FIFOInputBuffer::append(const float *left, const float *right, int size) {
  size_t until = m_data.size - size;
  for (size_t i = size; i < m_data.size; ++i) {
    m_data.left[i-size] = m_data.left[i];
    m_data.right[i-size] = m_data.right[i];
  }
  for (size_t i = 0; i < size; ++i) {
    m_data.left[i + until] = left[i];
    m_data.right[i + until] = right[i];;
  }
}
} // namespace TuneZ