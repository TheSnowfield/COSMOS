#include <stdio.h>
#include <string.h>

#include <sys/tick.h>
// #include <sys/display.h>
#include <sys/scheduler.h>
#include <sys/button.h>

#include <hardware/ch9329.h>
#include <hardware/norflash.h>

// #include <widgets/stack.h>
// #include <widgets/statbar.h>
// #include <layout/main/wave.h>
// #include <layout/main/lowbatt.h>
// #include <layout/eggs/gol.h>

// #include <embed/texture/logo.h>
// #include <embed/texture/font_roboto_12.h>

void appmain() {
  ch9329_init();
  // norflash_init();

  HAL_Delay(10000);
  // extern void CM0_FASTMULsBOX_AES_decrypt(uint8_t* rk, const uint8_t* in, uint8_t* out, size_t rounds);

  // uint8_t out=0;
  // CM0_FASTMULsBOX_AES_decrypt(NULL, NULL, &out, 0);
  // HAL_Delay(out);

	HAL_Delay(1000);

}
