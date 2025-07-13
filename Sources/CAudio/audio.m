#define MINIAUDIO_IMPLEMENTATION
#define MA_NO_RUNTIME_LINKING
#define MA_LOG_LEVEL 4
#define MA_DEBUG_OUTPUT


#include <string.h>
#include <unistd.h> // for getpid()
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include "miniaudio.h"
#include "audio.h"

typedef struct {
  float* buffer;
  unsigned int bufferSize;
  unsigned int bufferDataSize;
} Recording;


typedef struct {
  ma_device device;
  Recording* active_recording;
  unsigned int playback_position;
  Recording* active_playback;
} AudioContext;

static AudioContext audio_context;

void my_log_callback(void* pUserData, ma_uint32 level, const char* pMessage) {
    const char* levelStr = "";

    switch (level) {
        case MA_LOG_LEVEL_ERROR:   levelStr = "ERROR"; break;
        case MA_LOG_LEVEL_WARNING: levelStr = "WARN";  break;
        case MA_LOG_LEVEL_INFO:    levelStr = "INFO";  break;
        case MA_LOG_LEVEL_DEBUG:   levelStr = "DEBUG"; break;
        default:                   levelStr = "LOG";   break;
    }

    printf("[miniaudio][%s] %s\n", levelStr, pMessage);
}

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

}

int calc_buffer_size(int seconds) {
  return 48000 * seconds;
}


static ma_context context;
static ma_log logger;
ma_log_callback logCallback;

/******************** [external api start] ************************************/


int audio_setup() {
  ma_log_init(NULL, &logger);
  logCallback = ma_log_callback_init(my_log_callback, NULL);
  ma_log_init(NULL, &logger);
  ma_log_register_callback(&logger, logCallback);


  ma_device_config config = ma_device_config_init(ma_device_type_duplex);
  config.capture.format = ma_format_f32;
  config.capture.channels = 1;
  config.playback.format = ma_format_f32;
  config.playback.channels = 1;
  config.sampleRate = 48000;

  config.dataCallback = rec_callback;

  config.pUserData = &audio_context;

  if (MA_SUCCESS != ma_device_init(NULL, &config, &audio_context.recorder)) {
    return -1;
  }

  // ma_device_config play_config = ma_device_config_init(ma_device_type_playback);
  // play_config.playback.format = ma_format_f32;
  // play_config.playback.channels = 1;
  // play_config.sampleRate = 48000;
  // play_config.dataCallback = play_callback;
  // play_config.pUserData = &audio_context;
  //
  // if (MA_SUCCESS != ma_device_init(NULL, &play_config, &audio_context.player)) {
  //   return -1;
  // }

  return 0;
}

Recording rec;

void create_recording(unsigned int seconds) {
  rec = (Recording) {
    .bufferDataSize = 0,
    .bufferSize = calc_buffer_size(seconds),
  };
  rec.buffer = malloc(rec.bufferSize * sizeof(float));
  printf("🔊 recording created with size of %d for %d seconds\n", rec.bufferSize, seconds);
}

// void record(void (*meter)(float)) { 
void record() { 
  printf("🔊 starting to record...\n");
  audio_context.active_recording = &rec;
  // audio_context.meter = meter;
  unsigned int result = ma_device_start(&audio_context.recorder);
  if (MA_SUCCESS != result) {
    printf("⚠️ starting device failed! %d\n", result);
    return;
  }
}

// void play(void (*meter)(float)) { 
void play() { 
  printf("🔊 starting playback...\n");
  audio_context.playback_position = 0;
  // audio_context.meter = meter;
  audio_context.active_playback = &rec;
  if (MA_SUCCESS != ma_device_start(&audio_context.player)) {
    printf("⚠️ starting device failed!\n");
    return;
  }
}

void audio_teardown() {
  ma_device_uninit(&audio_context.recorder);
  ma_device_uninit(&audio_context.player);
  if (rec.buffer) {
    free(rec.buffer);
  }
}


