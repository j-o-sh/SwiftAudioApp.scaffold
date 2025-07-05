#pragma once

typedef struct {
  float* buffer;
  unsigned int bufferSize;
  unsigned int bufferDataSize;
} Recording;

Recording create_recording();

int calc_buffer_size(int seconds);

int audio_setup();

void audio_teardown();

void record(Recording* record, void (*meter)(float));

void play(Recording* rec, void (*meter)(float));

