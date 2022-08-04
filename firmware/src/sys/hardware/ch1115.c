#define _CH1115_INTERNAL
#include "ch1115.h"
#undef _CH1115_INTERNAL

#include <stdlib.h>
#include <string.h>

#define Y2PAGE(y) (y / 8)
#define X2COLUMN_L(x) (x & 0x00FF)
#define X2COLUMN_H(x) (x >> 4)

status_t ch1115_write_cmd(uint8_t cmd, uint8_t* args, uint8_t len) {
  uint8_t buffer[Ch1115_MAX_ARGLEN] = {0}; {
    if(len > Ch1115_MAX_ARGLEN) return error;

    // set command type and cmd
    buffer[0] = cmd_type_operation;
    buffer[1] = cmd;

    // copy the data of args
    if(args != NULL && len != 0)
      memcpy(buffer + 2, args, len);
  }

  HAL_StatusTypeDef stat;
  stat = HAL_I2C_Master_Transmit(&i2c1, CH1115_ADDRESS, buffer, len + 2, HAL_MAX_DELAY);

  return !(stat == HAL_OK);
}

void ch1115_init() {

  HAL_GPIO_Init(I2C1_PORT, &port_i2c1);
  HAL_GPIO_Init(I2C1_PORT, &port_i2c1_ctl);

  __HAL_RCC_I2C1_CLK_ENABLE(); {
    HAL_I2C_Init(&i2c1);
    HAL_I2CEx_ConfigAnalogFilter(&i2c1, I2C_ANALOGFILTER_ENABLE);
    HAL_I2CEx_ConfigDigitalFilter(&i2c1, 0);
  }

  // reset display
  ch1115_reset_reset(); HAL_Delay(10);
  ch1115_reset_set();   HAL_Delay(10);

  // initialization
  for(int i = 0; i < sizeof(ch1115_init_cmd) / sizeof(ch1115_initcmd_t); ++i) {

    // transmit cmd
    ch1115_write_cmd(ch1115_init_cmd[i].cmd, ch1115_init_cmd[i].args, ch1115_init_cmd[i].len);

    // delay if needed
    if(ch1115_init_cmd[i].delay != 0)
      HAL_Delay(ch1115_init_cmd[i].delay);
  }

  ch1115_write_cmd(CH1115_REG_SET_PAGEADD + 0, NULL, 0);
  ch1115_write_cmd(CH1115_REG_SET_COLADD_LSB + 0, NULL, 0);

}

// void ch1115_init_dma(bool mode) {
//   __HAL_RCC_DMA1_CLK_ENABLE();
//   HAL_NVIC_SetPriority(DMA1_Channel2_3_IRQn, 0, 0);
//   HAL_NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);

//   // initialize DMA
//   dma_i2c1.Init.Mode = mode ? DMA_CIRCULAR : DMA_NORMAL;
//   HAL_DMA_Init(&dma_i2c1);

//   // link DMA with I2C
//   __HAL_LINKDMA(&i2c1, hdmatx, dma_i2c1);
// }

// void ch1115_write_bytes_dma(uint8_t *data, uint32_t len) {
//   HAL_I2C_Master_Transmit_DMA(&i2c1, CH1115_ADDRESS, data, len);
// }

status_t ch1115_light_on() {
  return ch1115_write_cmd(CH1115_REG_DISPLAY_ON, NULL, 0);
}

status_t ch1115_light_off() {
  return ch1115_write_cmd(CH1115_REG_DISPLAY_OFF, NULL, 0);
}

status_t ch1115_set_pixel(uint8_t x, uint8_t y, ch1115_color_t color) {
  ch1115_write_cmd(CH1115_REG_SET_PAGEADD + Y2PAGE(y), NULL, 0);
  ch1115_write_cmd(CH1115_REG_SET_COLADD_LSB + X2COLUMN_L(x), NULL, 0);
  ch1115_write_cmd(CH1115_REG_SET_COLADD_MSB + X2COLUMN_H(x), NULL, 0);
  HAL_I2C_Master_Transmit(&i2c1, CH1115_ADDRESS, (uint8_t[]){ 0x40, ~(color ^ (1 << (y & 0x0F))) }, 2, HAL_MAX_DELAY);
}

status_t ch1115_set_pixel_column(uint8_t bits) {
  HAL_I2C_Master_Transmit(&i2c1, CH1115_ADDRESS, (uint8_t[]){ 0x40, bits }, 2, HAL_MAX_DELAY);
}

status_t ch1115_set_page(uint8_t page) {
  ch1115_write_cmd(CH1115_REG_SET_PAGEADD + page, NULL, 0);
}

status_t ch1115_set_column(uint8_t column) {
  ch1115_write_cmd(CH1115_REG_SET_COLADD_LSB + X2COLUMN_L(column), NULL, 0);
  ch1115_write_cmd(CH1115_REG_SET_COLADD_MSB + X2COLUMN_H(column), NULL, 0);
}

status_t ch1115_clear(ch1115_color_t color) {

  uint8_t clr[] = { 0x40, color, color, color, color, color, color, color, color };

  for(uint8_t page = 0; page < CH1115_HEIGHT_PAGE; ++page) {
    ch1115_set_page(page);
    ch1115_set_column(0);
    for(uint8_t column = 0; column < CH1115_WIDTH / 8; ++column) {
      HAL_I2C_Master_Transmit(&i2c1, CH1115_ADDRESS, clr, sizeof(clr), HAL_MAX_DELAY);
    }
  }

  return ok;
}
