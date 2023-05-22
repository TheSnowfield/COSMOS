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
#define I2C1_PIN_CTL_RST GPIO_PIN_8

#define USART_PORT GPIOB
#define USART_PIN_TX GPIO_PIN_6
#define USART_PIN_RX GPIO_PIN_7
#define USART_PIN_CTL_RST GPIO_PIN_5
#define USART_BAUDRATE 9600

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

extern GPIO_InitTypeDef port_swd;

// usart1 is used for ch9329
////////////////////////////////////////////////////////

extern GPIO_InitTypeDef port_usart1;
extern GPIO_InitTypeDef port_usart1_ctl;
extern UART_HandleTypeDef usart1;

// I2C1 is used for ch1115 oled display
////////////////////////////////////////////////////////

extern GPIO_InitTypeDef port_i2c1;
extern GPIO_InitTypeDef port_i2c1_ctl;
extern I2C_HandleTypeDef i2c1;
extern DMA_HandleTypeDef dma_i2c1;

// SPI1 is used for the spi nor flash
////////////////////////////////////////////////////////

extern SPI_HandleTypeDef spi1;
extern GPIO_InitTypeDef port_spi1_ctl;
extern GPIO_InitTypeDef port_spi1;

// Button keys
////////////////////////////////////////////////////////

extern GPIO_InitTypeDef port_buttons;
extern GPIO_InitTypeDef port_led;

#endif /* _SYS_HARDWARE_CONFIG_H */
