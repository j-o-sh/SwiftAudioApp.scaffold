#pragma once
#include "./project.c"


TUI_Meter playback_meter;
TUI_Meter record_meter;

void update_playback_meter(float value) {
  playback_meter.value = value;
  tui_update_meter(playback_meter);
}

void update_record_meter(float value) {
  record_meter.value = value;
  tui_update_meter(record_meter);
}

int main() {
  #ifdef DEBUG
  printf("Waiting for debugger... PID is %d\n", getpid());
  // raise(SIGSTOP);
  printf("When debugger attached, press enter.\n\n");
  getchar();
  #endif

  playback_meter = tui_create_meter("🔊");
  record_meter = tui_create_meter("🎙️");

  srand(time(NULL));
  int choice = 99;

  if (0 != audio_setup()) {
    return -1;
  }

  Recording recording = create_recording();
  recording.bufferSize = calc_buffer_size(5);
  recording.buffer = malloc(recording.bufferSize * sizeof(float));

  while (0 != choice) {
    printf("\n\n");
    printf("All the audio ✌️\n"
           "  1 - record a thing\n"
           "  2 - play the thing\n"
           "  0 - Exit\n");
    scanf("%d", &choice);
    switch (choice) {
    case 1:
      tui_display_meter(record_meter);
      record(&recording, update_record_meter);
      getchar();
      break;
    case 2:
      tui_display_meter(playback_meter);
      play(&recording, update_playback_meter);
      getchar();
      break;
    }
  }
  
  free(recording.buffer);
  audio_teardown();
}
