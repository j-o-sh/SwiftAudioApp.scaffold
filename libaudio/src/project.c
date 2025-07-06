#pragma once

#include <unistd.h> // for getpid()
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

#define MINIAUDIO_IMPLEMENTATION
#include "../external/miniaudio.h"
#include "../audio.h"
#include "./tui.c"
#include "./recording.c"

