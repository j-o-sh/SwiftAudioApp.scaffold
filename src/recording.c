#pragma once
#include "./project.c"

typedef struct {
  float* buffer;
  unsigned int bufferSize;
  unsigned int bufferDataSize;
} Recording;

typedef struct {
  ma_device recorder;
  Recording* active_recording;
  TUI_Meter recorder_meter;

  ma_device player;
  unsigned int playback_position;
  Recording* active_playback;
  TUI_Meter player_meter;
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
  ctx->recorder_meter.value = sum / frameCount;
  tui_update_meter(ctx->recorder_meter);

  if (bufferSizeLeft <= framesToWrite) {
    ma_device_stop(device);
    printf("\ndone.\n");
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
  ctx->player_meter.value = sum / framesToCopy;
  tui_update_meter(ctx->player_meter);
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
  audio_context.recorder_meter = tui_create_meter("🎙️");
  audio_context.player_meter = tui_create_meter("🔊");

  ma_device_config rec_config = ma_device_config_init(ma_device_type_capture);
  rec_config.capture.format = ma_format_f32;
  rec_config.capture.channels = 1;
  rec_config.sampleRate = 48000;
  rec_config.dataCallback = rec_callback;
  rec_config.pUserData = &audio_context;

  if (MA_SUCCESS != ma_device_init(NULL, &rec_config, &audio_context.recorder)) {
    printf("‼️ recording device initialization failed!");
    return -1;
  }

  ma_device_config play_config = ma_device_config_init(ma_device_type_playback);
  play_config.playback.format = ma_format_f32;
  play_config.playback.channels = 1;
  play_config.sampleRate = 48000;
  play_config.dataCallback = play_callback;
  play_config.pUserData = &audio_context;

  if (MA_SUCCESS != ma_device_init(NULL, &play_config, &audio_context.player)) {
    printf("‼️ playback device initialization failed!");
    return -1;
  }

  return 0;
}

void audio_teardown() {
  ma_device_uninit(&audio_context.recorder);
  ma_device_uninit(&audio_context.player);
}


void record(Recording* record) { 
  printf("Recording...\n"); 

  audio_context.active_recording = record;
  if (MA_SUCCESS != ma_device_start(&audio_context.recorder)) {
    printf("‼️ starting device failed!");
    return;
  }

  printf("🔥 lets go!\n");

  tui_display_meter(audio_context.recorder_meter);
}

void play(Recording* rec) { 
  printf("Playing...\n"); 

  audio_context.playback_position = 0;
  audio_context.active_playback = rec;
  tui_display_meter(audio_context.player_meter);
  if (MA_SUCCESS != ma_device_start(&audio_context.player)) {
    printf("‼️ starting playback failed!");
    return;
  }

  printf("\n\n");
}


