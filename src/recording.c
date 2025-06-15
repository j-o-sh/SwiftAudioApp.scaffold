#pragma once
#include "./project.c"

typedef struct {
  /**
   * 0 = uninitialized
   * 1 = success
   * -1 = Fail: Device Issues
   */
  int status;
  float* buffer;
  unsigned int bufferSize;
  unsigned int bufferDataSize;

  TUI_Meter meter;
  ma_device device;
} Recording;

void rec_callback(
  ma_device* device, 
  void* output, 
  const void* input, 
  ma_uint32 frameCount) {

  Recording* data = device->pUserData;
  const float* samples = (const float*) input;

  unsigned int bufferSizeLeft = data->bufferSize - data->bufferDataSize;
  unsigned int framesToWrite = frameCount < bufferSizeLeft ? frameCount : bufferSizeLeft;

  printf("-- %d / %d --\n", data->bufferDataSize, data->bufferSize);
  memcpy(
    &data->buffer[data->bufferDataSize], 
    input, 
    framesToWrite * sizeof(float)
  );
  data->bufferDataSize += framesToWrite;

  float sum = 0.0f;
  for (unsigned int i = 0; i < frameCount; i++) {
    sum += fabsf(samples[i]);
  }
  data->meter.value = sum / frameCount;
  tui_update_meter(data->meter);

  if (bufferSizeLeft <= framesToWrite) {
    ma_device_uninit(device);
    data->status = 1;
  }
}

Recording create_recording() {
  Recording record = {
    .status = 0,
    .bufferDataSize = 0,
    .meter = tui_create_meter("🎙️")
  };
  
  return record;
}

int calc_buffer_size(int seconds) {
  return 48000 * sizeof(float) * seconds;
}

void record(Recording* record) { 
  printf("Recording...\n"); 

  ma_device_config config = ma_device_config_init(ma_device_type_capture);
  config.capture.format = ma_format_f32;
  config.capture.channels = 1;
  config.sampleRate = 48000;
  config.dataCallback = rec_callback;
  config.pUserData = record;

  if (MA_SUCCESS != ma_device_init(NULL, &config, &record->device)) {
    printf("‼️ device initialization failed!");
    record->status = -1;
    return;
  }

  if (MA_SUCCESS != ma_device_start(&record->device)) {
    printf("‼️ starting device failed!");
    record->status = -1;
    return;
  }

  printf("🔥 lets go!\n");

  tui_display_meter(record->meter);
}

void play(Recording rec) { printf("Playing...\n"); }

