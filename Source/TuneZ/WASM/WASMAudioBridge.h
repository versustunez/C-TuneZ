#pragma once
#include "Audio/FFT.h"
#include "Audio/FIFOBuffer.h"
#include "Audio/FIFOInputBuffer.h"
#include "JSUtils/Logger.h"

#include <emscripten/emscripten.h>

extern "C" {
EMSCRIPTEN_KEEPALIVE
void audio_upload_data(float *left, float *right, int size) {
  auto &fifo = TuneZ::FIFOBuffer::Get();
  fifo.set(left, right, size);
  LOG("Setting Audio Data: {}", size);
}

EMSCRIPTEN_KEEPALIVE
void audio_data_ready() {
  auto &fifo = TuneZ::FIFOBuffer::Get();
  TuneZ::FFT::Get().process();
  fifo.setHasNewData(true);
  auto &io = TuneZ::FIFOInputBuffer::Get();
  io.append(fifo.data().left, fifo.data().right, fifo.dataSize());
}

EMSCRIPTEN_KEEPALIVE
void audio_set_max_decibels(float dB) {
  TuneZ::FFT::Get().SetMaxDB(dB);
}
}