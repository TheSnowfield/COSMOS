#include <config.h>

#define __NORFLASH_INTERNAL
#include <hardware/w25qx.h>
#undef __NORFLASH_INTERNAL

void w25qx_init() {

  // Initialize spi peripheral
  HAL_GPIO_Init(SPI1_PORT, &port_spi1);

  // Initialize spi control pins
  HAL_GPIO_Init(SPI1_PORT, &port_spi1_ctl);

  __HAL_RCC_SPI1_CLK_ENABLE();
  HAL_SPI_Init(&spi1);

	// reset chip
  w25qx_unselect();
	w25qx_unreset();
	
  w25qx_select(); {

  uint8_t buffer[] = {
    0x90, 0x00, 0x00, 0x00, 0x00, 0x00
  };

    HAL_SPI_TransmitReceive(&spi1, buffer, buffer, 6, HAL_MAX_DELAY);

    // uint8_t uniqid[8];
    // HAL_SPI_Receive(&spi1, (uint8_t *)&uniqid, sizeof(uniqid), HAL_MAX_DELAY);

    w25qx_unselect();
  }
}
