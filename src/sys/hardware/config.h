#ifndef _SYS_HARDWARE_CONFIG_H
#define _SYS_HARDWARE_CONFIG_H

#include <stm32f0xx.h>
#include <stm32f0xx_hal.h>
#include <stm32f0xx_hal_i2c.h>
#include <stm32f0xx_hal_i2c_ex.h>
#include <stm32f0xx_hal_spi.h>
#include <stm32f0xx_hal_gpio.h>
#include <stm32f0xx_hal_dma.h>
#include <stm32f0xx_hal_dma_ex.h>
#include <stm32f0xx_hal_tim.h>
#include <stm32f0xx_hal_tim_ex.h>

#define I2C1_PORT GPIOA
#define I2C1_PIN_SCL GPIO_PIN_9
#define I2C1_PIN_SDA GPIO_PIN_10

#define USART_PORT GPIOB
#define USART_PIN_TX GPIO_PIN_6
#define USART_PIN_RX GPIO_PIN_7
#define USART_PIN_CTL_RST GPIO_PIN_5
#define USART_BAUDRATE 115200

#define SPI1_PORT GPIOA
#define SPI1_PIN_SCK GPIO_PIN_5
#define SPI1_PIN_MISO GPIO_PIN_6
#define SPI1_PIN_MOSI GPIO_PIN_7
#define SPI1_PIN_CTL_RST GPIO_PIN_3
#define SPI1_PIN_CTL_CS GPIO_PIN_4

#define LED_PORT GPIOB
#define LED_PIN_BLINK GPIO_PIN_1

#define BUTTON_PORT GPIOA
#define BUTTON_PIN_KEY_A GPIO_PIN_11
#define BUTTON_PIN_KEY_B GPIO_PIN_12

static GPIO_InitTypeDef port_swd = {
  .Pin = GPIO_PIN_13 | GPIO_PIN_14
};

// usart1 is used for ch9329
////////////////////////////////////////////////////////

static GPIO_InitTypeDef port_usart1 = {
  .Pin = USART_PIN_TX | USART_PIN_RX,
  .Mode = GPIO_MODE_AF_PP,
  .Pull = GPIO_NOPULL,
  .Speed = GPIO_SPEED_FREQ_HIGH,
  .Alternate = GPIO_AF0_USART1
};

static GPIO_InitTypeDef port_usart1_ctl = {
  .Pin = USART_PIN_CTL_RST,
  .Mode = GPIO_MODE_OUTPUT_PP,
  .Pull = GPIO_NOPULL
};

static UART_HandleTypeDef usart1 = {
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

static GPIO_InitTypeDef port_i2c1 = {
  .Pin = I2C1_PIN_SCL | I2C1_PIN_SDA,
  .Mode = GPIO_MODE_AF_OD,
  .Pull = GPIO_NOPULL,
  .Speed = GPIO_SPEED_FREQ_HIGH,
  .Alternate = GPIO_AF4_I2C1
};

static I2C_HandleTypeDef i2c1 = {
  .Instance = I2C1,
  .Init.Timing = 0x2000090E,
  .Init.OwnAddress1 = 0,
  .Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT,
  .Init.DualAddressMode = I2C_DUALADDRESS_DISABLE,
  .Init.OwnAddress2 = 0,
  .Init.OwnAddress2Masks = I2C_OA2_NOMASK,
  .Init.GeneralCallMode = I2C_GENERALCALL_DISABLE,
  .Init.NoStretchMode = I2C_NOSTRETCH_DISABLE
};

// SPI1 is used for the spi nor flash
////////////////////////////////////////////////////////

static SPI_HandleTypeDef spi1 = {
  .Instance = SPI1,
  .Init.Mode = SPI_MODE_MASTER,
  .Init.Direction = SPI_DIRECTION_2LINES,
  .Init.DataSize = SPI_DATASIZE_4BIT,
  .Init.CLKPolarity = SPI_POLARITY_HIGH,
  .Init.CLKPhase = SPI_PHASE_2EDGE,
  .Init.NSS = SPI_NSS_SOFT,
  .Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128,
  .Init.FirstBit = SPI_FIRSTBIT_MSB,
  .Init.TIMode = SPI_TIMODE_DISABLE,
  .Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE,
  .Init.CRCPolynomial = 7,
  .Init.CRCLength = SPI_CRC_LENGTH_DATASIZE,
  .Init.NSSPMode = SPI_NSS_PULSE_DISABLE
};

static GPIO_InitTypeDef port_spi1_ctl = {
  .Pin = SPI1_PIN_CTL_RST | SPI1_PIN_CTL_CS,
  .Mode = GPIO_MODE_OUTPUT_PP,
  .Speed = GPIO_SPEED_FREQ_HIGH
};

static GPIO_InitTypeDef port_spi1 = {
  .Pin = SPI1_PIN_MISO | SPI1_PIN_MOSI | SPI1_PIN_SCK,
  .Mode = GPIO_MODE_AF_PP,
  .Pull = GPIO_NOPULL,
  .Speed = GPIO_SPEED_FREQ_HIGH,
  .Alternate = GPIO_AF0_SPI1
};

// Button keys
////////////////////////////////////////////////////////

static GPIO_InitTypeDef port_buttons = {
  .Pin = BUTTON_PIN_KEY_A | BUTTON_PIN_KEY_B,
  .Mode = GPIO_MODE_IT_RISING_FALLING,
  .Pull = GPIO_NOPULL
};

static GPIO_InitTypeDef port_led = {
  .Pin = LED_PIN_BLINK,
  .Mode = GPIO_MODE_OUTPUT_PP,
  .Pull = GPIO_NOPULL
};

#endif /* _SYS_HARDWARE_CONFIG_H */
