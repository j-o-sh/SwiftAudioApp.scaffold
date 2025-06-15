#pragma once
#include "./project.c"

int main() {
  srand(time(NULL));
  int choice = 99;

  Recording recording = create_recording();
  recording.bufferSize = calc_buffer_size(10);
  recording.buffer = malloc(recording.bufferSize);

  while (0 != choice) {
    printf("All the audio ✌️\n"
           "  1 - record a thing\n"
           "  2 - play the thing\n"
           "  0 - Exit\n");
    scanf("%d", &choice);
    switch (choice) {
    case 1:
      printf("We have a buffer %d/%d\n", recording.bufferDataSize, recording.bufferSize);
      record(&recording);
      getchar();
      printf("do it!\n");
      getchar();
      printf("...");

      break;
    case 2:
      // play((Recording) { .id = 9 });
      break;
    }
  }
  
  free(recording.buffer);
}
