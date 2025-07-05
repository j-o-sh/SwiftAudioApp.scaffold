#pragma once
#include "./project.c"

typedef struct TUI_Meter {
  char* name;
  float value;
} TUI_Meter;

TUI_Meter tui_create_meter(char* name) {
  return (TUI_Meter) { .name = name, .value = 0.0 };
}

void tui_display_meter(const TUI_Meter meter) {
  int scaled_value = meter.value * 15;
  const char *on = "●";
  const char *off = "○";
  const char *values[15];
  for (int i = 0; i < 15; i++) {
    values[i] = scaled_value < i + 1 ? off : on; 
  }
  // printf("%s: %.10s %.5s", meter.name, values, values + 10);
  printf("%s: ", meter.name);
  for (int i = 0; i < 15; i++) {
    printf("%s", values[i]);
    if (9 == i) {printf(" ");}
  }
  fflush(stdout);
}

void tui_clear_meter() {
  //basically we assume we still live on the meter line (as if nothing happened
  //to the output after calling display) so we need to clear the line we're on
  //or even better: clear backwards the number of characters that make up the 
  //meter display (length of name + 2 + 15)
  printf("\r"); // carriage return
  printf("\033[K"); // clear to end of line
  fflush(stdout);
}

void tui_update_meter(const TUI_Meter meter) {
  tui_clear_meter();
  tui_display_meter(meter);
}

