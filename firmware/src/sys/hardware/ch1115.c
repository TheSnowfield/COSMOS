#define _CH1115_INTERNAL
#include "ch1115.h"
#undef _CH1115_INTERNAL

#include <stdlib.h>
#include <string.h>

#define Y2PAGE(y) (y / 8)
#define X2COLUMN_L(x) (x & 0x00FF)
#define X2COLUMN_H(x) (x >> 4)

ch1115_display_ready_t __ch1115_display_ready = NULL;

void ch1115_set_readycb(ch1115_display_ready_t cb) {
  __ch1115_display_ready = cb;
}

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

  // ch1115_write_cmd(CH1115_REG_SET_PAGEADD + 0, NULL, 0);
  // ch1115_write_cmd(CH1115_REG_SET_COLADD_LSB + 0, NULL, 0);
}

status_t ch1115_light_on(bool breathing) {

  if(breathing) {
    // ch1115_write_cmd(CH1115_REG_CONTRAST_CONTROL, (uint8_t[]) { 0b00000000 }, 1);
    // ch1115_write_cmd(CH1115_REG_DISPLAY_ON, NULL, 0);
    // for (uint8_t i = 0; i < 255; i++) {
    //   ch1115_write_cmd(CH1115_REG_CONTRAST_CONTROL, &i, 1);
    //   HAL_Delay(3);
    // }

    ch1115_write_cmd(CH1115_REG_CONTRAST_CONTROL, (uint8_t[]) { 0b00000000 }, 1);
    ch1115_write_cmd(CH1115_REG_DISPLAY_ON, NULL, 0);
    ch1115_write_cmd(CH1115_REG_BREATHEFFECT_SET, (uint8_t[]) { 0b10000000 }, 1);
    HAL_Delay(650);
    ch1115_write_cmd(CH1115_REG_BREATHEFFECT_SET, (uint8_t[]) { 0b00000000 }, 1);
    ch1115_write_cmd(CH1115_REG_CONTRAST_CONTROL, (uint8_t[]) { 0b11111111 }, 1);
    return ok;
  }

  ch1115_write_cmd(CH1115_REG_DISPLAY_ON, NULL, 0);
  return ok;
}

status_t ch1115_light_off(bool breathing) {
  
  if(breathing) {
    ch1115_write_cmd(CH1115_REG_CONTRAST_CONTROL, (uint8_t[]) { 0b11111111 }, 1);
    ch1115_write_cmd(CH1115_REG_DISPLAY_ON, NULL, 0);
    for (uint8_t i = 255; i > 0 ; --i) {
      ch1115_write_cmd(CH1115_REG_CONTRAST_CONTROL, &i, 1);
      HAL_Delay(0);
    }
  }

  ch1115_write_cmd(CH1115_REG_DISPLAY_OFF, NULL, 0);
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
  return ch1115_write_cmd(CH1115_REG_SET_PAGEADD + page, NULL, 0);
}

status_t ch1115_set_column(uint8_t column) {
  ch1115_write_cmd(CH1115_REG_SET_COLADD_LSB + X2COLUMN_L(column), NULL, 0);
  return ch1115_write_cmd(CH1115_REG_SET_COLADD_MSB + X2COLUMN_H(column), NULL, 0);
}

status_t ch1115_write_page_it(uint8_t page, uint8_t* buf, uint8_t len) {
  ch1115_set_page(page);
  HAL_I2C_Master_Transmit_IT(&i2c1, CH1115_ADDRESS, buf, len);
  return ok;
}

status_t ch1115_write_page(uint8_t page, uint8_t* buf, uint8_t len) {
  ch1115_set_page(page);
  HAL_I2C_Master_Transmit(&i2c1, CH1115_ADDRESS, buf, len, HAL_MAX_DELAY);
  return ok;
}
