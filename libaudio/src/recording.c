#pragma once
#include "./project.c"
#include "../recorder.h"

typedef struct {
  ma_device recorder;
  Recording* active_recording;
  void (*meter)(float);

  ma_device player;
  unsigned int playback_position;
  Recording* active_playback;
} AudioContext;

static AudioContext audio_context;

void rec_callback(
  ma_device* device, 
  void* output, 
  const void* input, 
  ma_uint32 frameCount) {
  (void)output;

  AudioContext* ctx = device->pUserData;
  const float* samples = (const float*) input;

  unsigned int bufferSizeLeft = 
    ctx->active_recording->bufferSize - ctx->active_recording->bufferDataSize;
  unsigned int framesToWrite = 
    frameCount < bufferSizeLeft ? frameCount : bufferSizeLeft;

  memcpy(
    &ctx->active_recording->buffer[ctx->active_recording->bufferDataSize], 
    input, 
    framesToWrite * sizeof(float)
  );
  ctx->active_recording->bufferDataSize += framesToWrite;

  float sum = 0.0f;
  for (unsigned int i = 0; i < frameCount; i++) {
    sum += fabsf(samples[i]);
  }
  ctx->meter(sum / frameCount);

  if (bufferSizeLeft <= framesToWrite) {
    ma_device_stop(device);
  }
}

void play_callback(
  ma_device* device, 
  void* output, 
  const void* input, 
  ma_uint32 frameCount) {
  (void)input;

  AudioContext* ctx = device->pUserData;
  Recording* rec = ctx->active_playback;
  float* out = (float*) output;

  unsigned int framesLeft = rec->bufferDataSize - ctx->playback_position;
  unsigned int framesToCopy = frameCount <= framesLeft ? frameCount : framesLeft;

  memcpy(out, &rec->buffer[ctx->playback_position], framesToCopy * sizeof(float));
  ctx->playback_position += framesToCopy;

  if (framesToCopy < frameCount) {
    memset(out + framesToCopy, 0, (frameCount - framesToCopy) * sizeof(float));
  }

  if (ctx->playback_position >= rec->bufferDataSize) {
    ma_device_stop(device);
  }

  float sum = 0.0f;
  for (unsigned int i = 0; i < framesToCopy; i++) { 
    sum += fabsf(out[i]); 
  }
  ctx->meter(sum / framesToCopy);
}

Recording create_recording() {
  Recording record = {
    .bufferDataSize = 0,
  };
  
  return record;
}

int calc_buffer_size(int seconds) {
  return 48000 * seconds;
}

int audio_setup() {
  ma_device_config rec_config = ma_device_config_init(ma_device_type_capture);
  rec_config.capture.format = ma_format_f32;
  rec_config.capture.channels = 1;
  rec_config.sampleRate = 48000;
  rec_config.dataCallback = rec_callback;
  rec_config.pUserData = &audio_context;

  if (MA_SUCCESS != ma_device_init(NULL, &rec_config, &audio_context.recorder)) {
    return -1;
  }

  ma_device_config play_config = ma_device_config_init(ma_device_type_playback);
  play_config.playback.format = ma_format_f32;
  play_config.playback.channels = 1;
  play_config.sampleRate = 48000;
  play_config.dataCallback = play_callback;
  play_config.pUserData = &audio_context;

  if (MA_SUCCESS != ma_device_init(NULL, &play_config, &audio_context.player)) {
    return -1;
  }

  return 0;
}

void audio_teardown() {
  ma_device_uninit(&audio_context.recorder);
  ma_device_uninit(&audio_context.player);
}


void record(Recording* record, void (*meter)(float)) { 
  audio_context.active_recording = record;
  audio_context.meter = meter;
  if (MA_SUCCESS != ma_device_start(&audio_context.recorder)) {
    return;
  }
}

void play(Recording* rec, void (*meter)(float)) { 
  audio_context.playback_position = 0;
  audio_context.meter = meter;
  audio_context.active_playback = rec;
  if (MA_SUCCESS != ma_device_start(&audio_context.player)) {
    return;
  }
}


