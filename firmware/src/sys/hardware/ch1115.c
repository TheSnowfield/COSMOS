#define _CH1115_INTERNAL
#include "ch1115.h"
#undef _CH1115_INTERNAL

void ch1115_init() {

  __HAL_RCC_I2C1_CLK_ENABLE();
  HAL_GPIO_Init(I2C1_PORT, &port_i2c1);

  HAL_I2C_Init(&i2c1);
  HAL_I2CEx_ConfigAnalogFilter(&i2c1, I2C_ANALOGFILTER_ENABLE);
  HAL_I2CEx_ConfigDigitalFilter(&i2c1, 0);

  // Reset display
  ch1115_reset_set();

  // Initialize
  for(int i = 0; i < sizeof(ch1115_init_cmd) / sizeof(ch1115_initcmd_t); ++i) {

    // Transmit cmd
    HAL_I2C_Master_Transmit(&i2c1, CH1115_ADDRESS,
                            ch1115_init_cmd[i].cmd, ch1115_init_cmd[i].len, HAL_MAX_DELAY);

    // Delay
    if(ch1115_init_cmd[i].delay != 0)
      HAL_Delay(ch1115_init_cmd[i].delay);
  }

  uint8_t c[] = {0x40, 0xFF, 0xFF, 0xFF};

  HAL_I2C_Master_Transmit(&i2c1, CH1115_ADDRESS, c, sizeof(c), HAL_MAX_DELAY);
  
}
