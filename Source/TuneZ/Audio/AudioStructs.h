#pragma once

#include <kissfft/kiss_fft.h>
#include <vector>

namespace TuneZ {
struct Channel {
  float *left;
  float *right;
  int size{0};
};

struct MovingChannel {
  std::vector<float> left;
  std::vector<float> right;
  int size{0};
};

struct FFTChannelData {
  kiss_fft_cpx *in;
  kiss_fft_cpx *out;
  void setup(int size) {
    in = new kiss_fft_cpx[size];
    out = new kiss_fft_cpx[size];
  }

  ~FFTChannelData() {
    delete[] in;
    delete[] out;
  }
};
struct FFTChannel {
  FFTChannelData left{};
  FFTChannelData right{};
};
} // namespace TuneZ