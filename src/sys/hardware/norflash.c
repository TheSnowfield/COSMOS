#include <config.h>
#include <hardware/norflash.h>

void norflash_init() {

  // Initialize spi control pins
  HAL_GPIO_Init(SPI1_PORT, &port_spi1_ctl);

  // Initialize spi peripheral
  HAL_GPIO_Init(SPI1_PORT, &port_spi1);

  __HAL_RCC_SPI1_CLK_ENABLE();
  HAL_SPI_Init(&spi1);

  const char x[] = "2333";
  HAL_SPI_Transmit(&spi1, (uint8_t *)x, 3, HAL_MAX_DELAY);

}
