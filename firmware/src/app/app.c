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
#include <embed/texture/cosmos_binary_bmp.h>

void appmain() {

  display_bitblt(0, 0, 0, 0, 128, 32, resources_cosmos_binary_bmp);

  ch9329_init();
  w25qx_init();
}
