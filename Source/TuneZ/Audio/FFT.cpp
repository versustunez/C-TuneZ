#include "FFT.h"

#include "FIFOBuffer.h"

#define MIN_dB -70.0

namespace TuneZ {

FFT::FFT() {
  m_fft_data.left.setup(FFT_SIZE);
  m_fft_data.right.setup(FFT_SIZE);
  m_data.left = new float[FFT_SIZE_HALF];
  m_data.right = new float[FFT_SIZE_HALF];

  for (int i = 0; i < FFT_SIZE_HALF; ++i) {
    m_data.left[i] = MIN_dB;
    m_data.right[i] = MIN_dB;
  }
  m_window = new float[FFT_SIZE];
  // HANNING WINDOWING;
  for(int i=0; i<FFT_SIZE; i++)
    m_window[i] = 0.5 * (1 - cos(2*M_PI*(i+1) / (FFT_SIZE+1)));


  m_cfg = kiss_fft_alloc(FFT_SIZE, false, 0, 0);
};
FFT::~FFT() {
  kiss_fft_free(m_cfg);
  delete[] m_data.left;
  delete[] m_data.right;
  delete[] m_window;
};
void FFT::process() {
  auto &fifo = FIFOBuffer::Get();
  auto &channelData = fifo.data();
  for (int i = 0; i < channelData.size; ++i) {
    m_fft_data.left.in[i].r = channelData.left[i] * m_window[i];
    m_fft_data.left.in[i].i = 0;
    m_fft_data.left.out[i].i = 0;
    m_fft_data.left.out[i].r = 0;

    m_fft_data.right.in[i].r = channelData.right[i] * m_window[i];
    m_fft_data.right.in[i].i = 0;
    m_fft_data.right.out[i].i = 0;
    m_fft_data.right.out[i].r = 0;
  }
  kiss_fft(m_cfg, m_fft_data.left.in, m_fft_data.left.out);
  kiss_fft(m_cfg, m_fft_data.right.in, m_fft_data.right.out);
  constexpr double scale = 1.0 / FFT_SIZE;
  for (int i = 0; i < FFT_SIZE_HALF; ++i) {
    auto &lItem = m_fft_data.left.out[i];
    auto &rItem = m_fft_data.right.out[i];
    double l = 20.0 * std::log10(std::abs(lItem.r * scale));
    double r = 20.0 * std::log10(std::abs(rItem.r * scale));
    double oldL = m_data.left[i];
    double oldR = m_data.right[i];
    m_data.left[i] = (std::clamp(l, MIN_dB, m_dB) + oldL) * 0.5;
    m_data.right[i] = (std::clamp(r, MIN_dB, m_dB) + oldR) * 0.5;
  }

  m_newDataReady = true;
}
void FFT::SetMaxDB(double dB) {
  m_dB = dB;
}
} // namespace TuneZ