#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "./tui.h"

typedef struct {
  int id;
} Recording;

void record() { 
  printf("Recording...\n"); 
  TUI_Meter meter = tui_create_meter("🎙️");
  tui_display_meter(meter);

  for(int i = 0; i < 1000; i++) {
    meter.value = (double)rand() / RAND_MAX;
    tui_update_meter(meter);
    usleep(50000);
  }
  printf("\n\n"); 
}

void play(Recording rec) { printf("Playing...\n"); }

