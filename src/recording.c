#pragma once
#include "./project.c"


typedef struct {
  int id;
} Recording;

Recording record() { 
  ma_encoder_config encoding = ma_encoder_config_init(
    ma_encoding_format_wav,
    ma_format_f32,
    1, 
    48000
  );

  printf("Recording...\n"); 
  TUI_Meter meter = tui_create_meter("🎙️");
  tui_display_meter(meter);

  for(int i = 0; i < 100; i++) {
    meter.value = (double)rand() / RAND_MAX;
    tui_update_meter(meter);
    usleep(50000);
  }
  printf("\n\n"); 
  return (Recording){ .id = 1 };
}

void play(Recording rec) { printf("Playing...\n"); }

