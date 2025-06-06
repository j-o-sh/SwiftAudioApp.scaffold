#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
// #include "../external/miniaudio.h"
#include "./recording.h"



int main() {
  srand(time(NULL));
  int choice;
  Recording foo = {.id = 1};
  while (0 != choice) {
    printf("All the audio ✌️\n"
           "  1 - record a thing\n"
           "  2 - play the thing\n"
           "  0 - Exit\n");
    scanf("%d", &choice);
    switch (choice) {
    case 1:
      record();
      break;
    case 2:
      play((Recording) { .id = 9 });
      break;
    }
  }
}
