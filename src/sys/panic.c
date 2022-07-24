#include <stm32f0xx.h>

void panic() {

  // disable all interrupts
  __disable_irq();

  for(;;);
}

// void assert(bool condition, const char* reason) {

// }
