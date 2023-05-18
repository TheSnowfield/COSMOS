#include "config.h"

GPIO_InitTypeDef port_swd = {
  .Pin = GPIO_PIN_13 | GPIO_PIN_14
};

// usart1 is used for ch9329
////////////////////////////////////////////////////////

GPIO_InitTypeDef port_usart1 = {
  .Pin = USART_PIN_TX | USART_PIN_RX,
  .Mode = GPIO_MODE_AF_PP,
  .Pull = GPIO_NOPULL,
  .Speed = GPIO_SPEED_FREQ_HIGH,
  .Alternate = GPIO_AF0_USART1
};

GPIO_InitTypeDef port_usart1_ctl = {
  .Pin = USART_PIN_CTL_RST,
  .Mode = GPIO_MODE_OUTPUT_PP,
  .Pull = GPIO_NOPULL
};

UART_HandleTypeDef usart1 = {
  .Instance = USART1,
  .Init.BaudRate = USART_BAUDRATE,
  .Init.WordLength = UART_WORDLENGTH_8B,
  .Init.StopBits = UART_STOPBITS_1,
  .Init.Parity = UART_PARITY_NONE,
  .Init.Mode = UART_MODE_TX_RX,
  .Init.HwFlowCtl = UART_HWCONTROL_NONE,
  .Init.OverSampling = UART_OVERSAMPLING_16,
  .Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE,
  .AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT
};

// I2C1 is used for ch1115 oled display
////////////////////////////////////////////////////////

GPIO_InitTypeDef port_i2c1 = {
  .Pin = I2C1_PIN_SCL | I2C1_PIN_SDA,
  .Mode = GPIO_MODE_AF_OD,
  .Pull = GPIO_NOPULL,
  .Speed = GPIO_SPEED_FREQ_HIGH,
  .Alternate = GPIO_AF4_I2C1
};

GPIO_InitTypeDef port_i2c1_ctl = {
  .Pin = I2C1_PIN_CTL_RST,
  .Mode = GPIO_MODE_OUTPUT_PP,
  .Speed = GPIO_SPEED_FREQ_HIGH
};

I2C_HandleTypeDef i2c1 = {
  .Instance = I2C1,
  .Init.Timing = 0x00000001,
  .Init.OwnAddress1 = 0,
  .Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT,
  .Init.DualAddressMode = I2C_DUALADDRESS_DISABLE,
  .Init.OwnAddress2 = 0,
  .Init.OwnAddress2Masks = I2C_OA2_NOMASK,
  .Init.GeneralCallMode = I2C_GENERALCALL_DISABLE,
  .Init.NoStretchMode = I2C_NOSTRETCH_DISABLE
};

DMA_HandleTypeDef dma_i2c1 = {
  .Instance = DMA1_Channel2,
  .Init.Direction = DMA_MEMORY_TO_PERIPH,
  .Init.PeriphInc = DMA_PINC_DISABLE,
  .Init.MemInc = DMA_MINC_ENABLE,
  .Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE,
  .Init.MemDataAlignment = DMA_PDATAALIGN_BYTE,
  .Init.Mode = DMA_NORMAL,
  .Init.Priority = DMA_PRIORITY_HIGH
};

// SPI1 is used for the spi nor flash
////////////////////////////////////////////////////////

SPI_HandleTypeDef spi1 = {
  .Instance = SPI1,
  .Init.Mode = SPI_MODE_MASTER,
  .Init.Direction = SPI_DIRECTION_2LINES,
  .Init.DataSize = SPI_DATASIZE_8BIT,
  .Init.CLKPolarity = SPI_POLARITY_LOW,
  .Init.CLKPhase = SPI_PHASE_1EDGE,
  .Init.NSS = SPI_NSS_SOFT,
  .Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16,
  .Init.FirstBit = SPI_FIRSTBIT_MSB,
  .Init.TIMode = SPI_TIMODE_DISABLE,
  .Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE,
  .Init.CRCPolynomial = 7,
  .Init.CRCLength = SPI_CRC_LENGTH_DATASIZE,
  .Init.NSSPMode = SPI_NSS_PULSE_DISABLE
};

GPIO_InitTypeDef port_spi1_ctl = {
  .Pin = SPI1_PIN_CTL_RST | SPI1_PIN_CTL_CS,
  .Mode = GPIO_MODE_OUTPUT_PP,
  .Speed = GPIO_SPEED_FREQ_HIGH
};

GPIO_InitTypeDef port_spi1 = {
  .Pin = SPI1_PIN_MISO | SPI1_PIN_MOSI | SPI1_PIN_SCK,
  .Mode = GPIO_MODE_AF_PP,
  .Pull = GPIO_NOPULL,
  .Speed = GPIO_SPEED_FREQ_HIGH,
  .Alternate = GPIO_AF0_SPI1
};

// Button keys
////////////////////////////////////////////////////////

GPIO_InitTypeDef port_buttons = {
  .Pin = BUTTON_PIN_KEY_A | BUTTON_PIN_KEY_B,
  .Mode = GPIO_MODE_IT_RISING_FALLING,
  .Pull = GPIO_NOPULL
};

GPIO_InitTypeDef port_led = {
  .Pin = LED_PIN_BLINK,
  .Mode = GPIO_MODE_OUTPUT_PP,
  .Pull = GPIO_NOPULL
};
