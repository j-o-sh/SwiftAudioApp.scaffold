#pragma once
#include "./project.c"

typedef struct {
  /**
   * 0 = uninitialized
   * 1 = success
   * -1 = Fail: Device Issues
   */
  int status;
} Recording;

typedef struct {
  Recording* record;
  TUI_Meter* meter;
} RecUserData ;

void rec_callback(
  ma_device* device, 
  void* output, 
  const void* input, 
  ma_uint32 frameCount) {

  RecUserData* data = device->pUserData;
  const float* samples = (const float*) input;
  
  for (int i = 0; i < frameCount; i++) {
    data->meter->value = fabsf(samples[i]);
    tui_update_meter(*data->meter);
  }
}

Recording record() { 
  printf("Recording...\n"); 
  Recording record = {};
  TUI_Meter meter = tui_create_meter("🎙️");
  
  ma_device_config config = ma_device_config_init(ma_device_type_capture);
  config.capture.format = ma_format_f32;
  config.capture.channels = 1;
  config.sampleRate = 48000;
  config.dataCallback = rec_callback;
  RecUserData data = { .record = &record, .meter = &meter };
  config.pUserData = &data;

  ma_device device;
  if (MA_SUCCESS != ma_device_init(NULL, &config, &device)) {
    printf("‼️ device initialization failed!");
    record.status = -1;
    return record;
  }
  if (MA_SUCCESS != ma_device_start(&device)) {
    printf("‼️ starting device failed!");
    record.status = -1;
    return record;
  }

  tui_display_meter(meter);

  for(int i = 0; i < 100; i++) {
    meter.value = (double)rand() / RAND_MAX;
    tui_update_meter(meter);
    usleep(50000);
  }
  printf("\n\n"); 

  getchar();

  ma_device_uninit(&device);
  return record;
}

void play(Recording rec) { printf("Playing...\n"); }

