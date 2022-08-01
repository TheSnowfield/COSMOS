#include <hardware/config.h>
#include <sys/scheduler.h>
// #include <sys/display.h>
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

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) panic();
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK) panic();
  if (HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit) != HAL_OK) panic();
  
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

void gpio_reconfigure() {

  HAL_GPIO_DeInit(GPIOA, GPIO_PIN_All ^ port_swd.Pin);
  HAL_GPIO_DeInit(GPIOB, GPIO_PIN_All);
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
}

// int fputc(int ch, FILE *f) {
//   HAL_UART_Transmit(&usart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
//   return ch;
// }

int main() {

  HAL_Init(); {

    // Reconfigure RCC
    rcc_reconfigure();

    // Reconfigure GPIO
    gpio_reconfigure();

    // Init scheduler
    scheduler_init();

    // Enable display
    // display_light(true);
    // display_init(false); {
    //   HAL_Delay(500); // avoid display tearing
    //   display_light(false);
    // }
  }

  // Call appmain
  appmain();
  while(1) scheduler_handler();
}
