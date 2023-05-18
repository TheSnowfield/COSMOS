#include <stdio.h>

#include <stm32f0xx.h>
#include <sys/display.h>

#pragma GCC push_options
#pragma GCC optimize ("O0")

void panic(const char *msg) {

  // // flush display
  // if(display_inited() && msg) {

  //   // clear display
  //   display_clear(0x000000);

  //   // draw logo
  //   display_bitblt(33, 20, 0, 0, 63, 19, logo);

  //   // draw message
  //   display_draw_string_ex(FONT_ROBOTO_12, 8, 51, "= SYSTEM PANIC =");
  //   display_draw_string_ex(FONT_ROBOTO_12, 8, 67, "================");
  //   display_draw_string_ex(FONT_ROBOTO_12, 0, 67 + 16, msg);
  // }

  // disable all interrupts
  __disable_irq();

  for(;;);
}

void assert(uint8_t condition, const char *msg) {
  char buf[128];
  snprintf(buf, sizeof(buf), "ASSERT FAILED: \n%s", msg);

  if (!condition) panic(buf);
}

#pragma GCC pop_options

void reset() {
  NVIC_SystemReset();
}

