#include <stdio.h>
#include <string.h>

#include <sys/tick.h>
#include <sys/display.h>
#include <sys/scheduler.h>
#include <sys/button.h>

#include <hardware/ch9329.h>
#include <hardware/w25qx.h>
#include <hardware/ch1115.h>

// #include <widgets/stack.h>
// #include <widgets/statbar.h>
// #include <layout/main/wave.h>
// #include <layout/main/lowbatt.h>
// #include <layout/eggs/gol.h>

// #include <embed/texture/logo.h>
// #include <embed/texture/font_roboto_12.h>

void appmain() {
  ch9329_init();
  w25qx_init();
}
