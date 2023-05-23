#include <hardware/config.h>
#include <sys/scheduler.h>
#include <sys/display.h>
#include <sys/panic.h>
#include <app/app.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

void rcc_reconfigure() {

  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_HSI14
                                   | RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.HSI14CalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL3;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                               |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  /**
   * Initializes the periph clocks
   */
  RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1 | RCC_PERIPHCLK_I2C1;
  RCC_PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  RCC_PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) panic(NULL);
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK) panic(NULL);
  if (HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit) != HAL_OK) panic(NULL);
  
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

void gpio_reconfigure() {

  HAL_GPIO_DeInit(GPIOA, GPIO_PIN_All ^ port_swd.Pin);
  HAL_GPIO_DeInit(GPIOB, GPIO_PIN_All);
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  // initialize spi
  __HAL_RCC_SPI1_CLK_ENABLE(); {
    HAL_GPIO_Init(SPI1_PORT, &port_spi1);
    HAL_GPIO_Init(SPI1_PORT, &port_spi1_ctl);
    HAL_SPI_Init(&spi1);
  }

  // initialize usart1
  __HAL_RCC_USART1_CLK_ENABLE(); {
    HAL_GPIO_Init(USART_PORT, &port_usart1_ctl);
    HAL_GPIO_Init(USART_PORT, &port_usart1);
    HAL_UART_Init(&usart1);
  }

  // initialize i2c1
  __HAL_RCC_I2C1_CLK_ENABLE(); {
    HAL_GPIO_Init(I2C1_PORT, &port_i2c1);
    HAL_GPIO_Init(I2C1_PORT, &port_i2c1_ctl);

    HAL_I2C_Init(&i2c1);
    HAL_I2CEx_ConfigAnalogFilter(&i2c1, I2C_ANALOGFILTER_ENABLE);
    HAL_I2CEx_ConfigDigitalFilter(&i2c1, 0);
    HAL_I2CEx_EnableFastModePlus(I2C_FASTMODEPLUS_I2C1);

    HAL_NVIC_SetPriority(I2C1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(I2C1_IRQn);
  }

  // configure button ports
  HAL_GPIO_Init(BUTTON_PORT, &port_buttons); {
    HAL_NVIC_SetPriority(EXTI4_15_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
  }
}

// int fputc(int ch, FILE *f) {
//   HAL_UART_Transmit(&usart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
//   return ch;
// }

int main() {

  HAL_Init(); {

    // reconfigure rcc
    rcc_reconfigure();

    // reconfigure gpio
    gpio_reconfigure();

    // init scheduler
    scheduler_init();
  }

  // call appmain
  scheduler_call_proc(appmain);

  // run scheduler
  while(1) scheduler_handler();
}
