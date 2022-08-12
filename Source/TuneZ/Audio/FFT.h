#pragma once
#include "AudioStructs.h"

#include <TypeDefs.h>
#include <kissfft/kiss_fft.h>

#define FFT_SIZE 2048
#define FFT_SIZE_HALF 1024
#define MAX_dB 0.0

namespace TuneZ {

class FFT : public Instance<FFT> {
public:
  FFT();
  ~FFT();
  void process();

  Channel &data() { return m_data; }
  float *left() { return m_data.left; }
  float *right() { return m_data.right; }

  int size() { return FFT_SIZE_HALF; }
  bool dataReady() { return m_newDataReady; }
  // this is for Rendering... so the Player Plugins can update only if needed :D
  void clearReadyFlag() { m_newDataReady = false; }

  void SetMaxDB(double dB);

protected:
  double m_dB{MAX_dB};
  Channel m_data{};
  FFTChannel m_fft_data{};
  kiss_fft_cfg m_cfg;
  float *m_window;
  bool m_newDataReady{false};
};

} // namespace TuneZ
